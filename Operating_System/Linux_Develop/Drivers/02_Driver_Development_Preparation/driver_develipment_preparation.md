# 驱动开发准备工作
需要准备交叉编译工具，Linux内核与设备树的确定等
## 编译Linux内核
boot.img就是将编译出来的Image和.dtb打包在一起，.dtb 就是.dts 设备树文件编译出来的。所以我们后面
如果要修改设备树，那么修改的就是.dts 这个文件
### 关闭内核log时间戳
进入kernel目录
```bash 
# 配置内核
make ARCH=arm64 rockchip_linux_defconfig
# 打开图形化菜单
make ARCH=arm64 menuconfig
```
```bash
-> Kernelhacking                                                     
    -> printk and dmesg options         
        ->Show timing information on printks   
```
## 根文件系统
## ADB向开发板发送下拉文件
## 交叉编译工具