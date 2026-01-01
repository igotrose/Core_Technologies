# 设备属下的LED实验
## 程序编写
### 修改设备树文件
- 在`\`根节点下面创建一个`rk3568_led`的子节点，在`dtsi`文件中输入以下内容
```dts
rk3568_led {
    compatible = "atkrk3568-led";
    status = "okay";
    reg = <0x0 0xFDC20010 0x0 0x08
            0x0 0xFDC2009 0x0 0x08
            0x0 0xFDC60004 0x0 0x08
            0x0 0xFDC6000C 0x0 0x08>;
};
```
### 驱动程序
```c
/* 获取设备树中的属性数据 */
/* 1、获取设备节点 */
dtsled.nd = of_find_node_by_path("/rk3568_led");
if (dtsled.nd == NULL)
{
    printk("rk3568_led node not find\r\n");
    goto fail_find_node;
}
else
{
    printk("rk3568_led node find\r\n");
}

/* 2、获取compatible属性内容 */
proper = of_find_property(dtsled.nd, "compatible", NULL);
if (proper == NULL)
{
    printk("compatible property find dailed\r\n");
}
else
{
    printk("compatible = %s\r\n", (char*)proper->value);
}

/* 3、获取status属性内容 */
ret_value = of_property_read_string(dtsled.nd, "status", &str);
if (ret_value < 0)
{
    printk("status read failed \r\n");
}
else
{
    printk("status = %s\r\n", str);
}

/* 4、获取reg属性内容 */
ret_value = of_property_read_u32_array(dtsled.nd, "reg", regdata, 16);
if (ret_value < 0)
{
    printk("reg property read failed\r\n");
}
else
{
    u8 i = 0;
    printk("reg data: \r\n");
    for (i = 0; i < 16; i++)
    {
        printk("%#X ", regdata[i]);
    }
    printk("\r\n");
}
```
### 加载并测试驱动
1. 挂载驱动
    ```bash
    depmod
    modprobe dtsled
    ls /dev/dtsled -l
    ```
2. 测试驱动
    ```bash
    ./ledApp /dev/dtsled 1
    ./ledApp /dev/dtsled 0
    ```