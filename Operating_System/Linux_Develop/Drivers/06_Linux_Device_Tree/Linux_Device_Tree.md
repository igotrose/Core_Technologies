# Linux设备树
## 设备树简介
`Device Tree`是一种用于描述硬件配置的数据结构，广泛应用于嵌入式系统如Linux内核中，用于将硬件的详细信息传递给操作系统，而无需在代码中硬编码。
## DTS、DTB和DTC
DTS是设备树源码文件，DTB是将DTS编译以后得到的二进制文件，DTC是编译DTS生产DTB的工具
- 编译DTS文件需要进入到内核源码根目录，执行以下命令
    - 编译linux源码中所有东西
        ```make ARCH=arm64 all```
    - 编译选中的所有设备树文件
        ```make ARCH=arm64 dtbs```
    - 编译某个单独的设备树文件
        ```make ARCH=arm64 xxx.dtb```

- 基于ARM架构的SOC有很多种，一种SOC又可以制作出很多款板子，每个板子都有一个对应的DTS文件，可以通过打开`arch/arm64/boot/dts/rockchip/Makefile`查看具体
## DTS语法
以`rk3568-evb2-lp4x-v10.dts`，`rk3568.dtsi`为例
### dtsi头文件
设备树里面除了可以通过`#include`来引用`.dtsi`文件，也可以引用`.h`文件头文件设备树也支持头文件，设备树的头文件扩展名为`.dtsi`，
```c
// rk3568-evb2-lp4x-v10.dts
#include "rk3568-evb2-lp4x-v10.dtsi"
#include "rk3568-android.dtsi"
// rk3568.dtsi
#include <dt-bindings/clock/rk3568-cru.h>
#include <dt-bindings/interrupt-controller/arm-gic.h>
#include <dt-bindings/interrupt-controller/irq.h>
#include <dt-bindings/pinctrl/rockchip.h>
#include <dt-bindings/soc/rockchip,boot-mode.h>
#include <dt-bindings/phy/phy.h>
#include <dt-bindings/power/rk3568-power.h>
#include <dt-bindings/soc/rockchip-system-status.h>
#include <dt-bindings/suspend/rockchip-rk3568.h>
#include <dt-bindings/thermal/thermal.h>
#include "rk3568-dram-default-timing.dtsi"
```
一般`.dtsi`文件用于描述SOC内部外设信息，如CPU架构主频、外设寄存器范围等
### 设备节点
一个设备节点对应这一个设备，这个节点通过一些属性信息描述节点信息
- 设备树根节点`/`，每个设备树文件都只有一个根节点
- 设备树中节点命名格式
    ```label:node-name@unit-address```
    - `label`标签的目的是为了方便访问
    - `node-name`是节点名字，为ASCII字符串，节点名字应该能够清晰的描述出节点的功能
    - `unit-address`一般表示设备的地址或寄存器首地址，如果某个节点没有地址或者寄存器的话可以不要

    `i2c0:i2c@ff3f0000`，节点label是i2c0，节点名字是i2c，寄存器首地址是ff3f0000
- 设备树源码中常用的几种数据形式
    1. 字符串（字符串列表）
    ```dtsi
    // 设置compatible属性的值为字符串rockchip,rk3588
    compatible = "rockchip,rk3588"; 
    // 设置compatible属性的值为字符串rockchip,rk3588-dwc3和rockchip,rk3399-dwc3
    compatible = "rockchip,rk3588-dwc3", "rockchip,rk3399-dwc3";  
    ```
    2. 32位无符号整数
    ```dtsi
    // reg属性的值为0
    reg = <0>; 
    // ，reg的值也可以设置为一组值
    reg = <0 0x123456 100>; 
    ```
### 标准属性
节点是由一堆的属性组成，节点都是具体的设备，不同的设备需要的属性不同
- **compatible** 属性
也叫做兼容性属性，他的值是一个字符串列表，用于将设备和驱动绑定起来。字符串列表用于选择设备所要使用的驱动程序，格式如下所示：
    ```
    manufacturer,model
    ```
    其中`manufacturer`表示厂商，`model`一般对用模块名字   
    - 单个属性值，假如有一个MIPI摄像头节点，这个节点的摄像头芯片采用的SONY公司出品的IMX415，compatible 属性值如下：
        ```
        compatible = "sony,imx425"
        ```
    - 多个属性值，设备会使用第一个兼容值在内核里查找对应的驱动文件，如果没有就使用下一个，直到查找完所有
        ```
        compatible = "ilitek,ili9881d","simple-panel-dsi";
        ```
    一般驱动程序文件都会有一个OF匹配表，此OF匹配表保存着一些compatible值，如果设备节点的compatible属性值和OF匹配表中的任何一个值相等，那么就表示设备可以使用这个驱动
    ```c
    static const struct of_device_id imx415_of_match[] = {
        {.compatible = "sony, imx415"},{},
    };
    ```
- **model** 属性
model属性值也是一个字符串，一般描述板字的名字或者设备模块信息
    ```
    model="Rockchip rk3568 EVB DDR4 V10 Board"
    ```
- **status** 属性
status属性值也是一个字符串，字符串是设备的状态信息，`okay`表面设备是可操作的，`disabled`表面设备是不可操作的
- **#address-cells** 和 **#size-cells** 属性 
这两属性的值都是无符号32位整型，并且都可以在任意子节点的设备中拥有，用于描述子节点的地址信息
    - `#address-cells`决定了子节点`reg`属性中地址信息所占用的字长（32位）
    - `#size-cells`属性值决定了子节点`reg`属性中长度信息所占的字长（32位）
- **reg** 属性
    reg 属性一般用于描述设备地址空间资源信息或者设备地址信息，其格式如下 
    ```
    reg=<address1 length1 address2 length2 address3 length3···>
    ```
    每个`address length`组合表示一个地址范围，`address`为其实地址，`length`为地址长度，`#address-cells`表明`address`这个数据所占用字长，`#size-cells`表明`length`这个数据所占用字长
- **ranges** 属性
该属性值可以为空或者按照以下格式编写的数字矩阵, 他是一个地址映射表
    ```
    ranges = <child-bus-address,parent-bus-address,length>
    ```
    - `child-bus-address` 子总线地址空间的物理地址，由父节点的`#address-cells`确定此物理地址所占用的字长
    - `parent-bus-address` 父总线地址空间的物理地址，由父节点的`#address-cells`确定此物理地址所占用的字长
    - `length` 子地址空间的长度，由父节点的`#size-cells`确定此物理地址所占用的字长

`ranges`属性为空值说明子地址空间和父地址空间完全相同，不需要进行地址转换
- **device_type** 属性
该属性值只能用于cpu或者memory，是字符串类型
### 根节点`compatible`属性
每个节点都有`compatible`属性，在`rk3566-evb2-lpx4-v10.dtsi`中，`compatible`的第一个值描述的是板子名称，第二个值描述的是设备所使用的SOC
```
/ {
	model = "Rockchip RK3566 EVB2 LP4X V10 Board";
	compatible = "rockchip,rk3566-evb2-lp4x-v10", "rockchip,rk3566";

	rk_headset: rk-headset {
```
在`include/linux/rockchip/cpu.h`中，`cpu_is_rk3566`函数会判断`compatible`的值是否匹配
```c
static inline bool cpu_is_rk3566(void)
{
	if (rockchip_soc_id)
		return (rockchip_soc_id & ROCKCHIP_CPU_MASK) == ROCKCHIP_CPU_RK3566;
	return of_machine_is_compatible("rockchip,rk3566");
}
```
### 向节点追加或者修改内容
假设现在需要加入一个新设备`fxls8471`，他需要使用到一个`i2c`的接口，建议在自己的板子的dtsi去添加，不影响其他板子，重点就是通过&label 来访问节点，然后直接在里面编写要追加或者修改的内容
```dts
&i2c1{
    status = "okay";
    clock-frequency = <400000>;
    fxls8471@1e{
        compatible = "fsl, fxls8471";
        reg = <0x1e>;
    }
}
```
## 设备树在系统中的体现
Linux内核启动的时候会解析设备树中各个节点的信息，并且在根文件系统的 `/proc/device-tree`目录下根据节点名字创建不同文件夹
1. 根节点的各个属性可以通过在`/proc/device-tree`的目录下`cat`查看
2. 根节点下面的子节点可以进入到对应的目录进行查看
## 特殊节点
在根节点下面有两个特殊的子节点：aliases和chosen
### aliases子节点
aliases 节点的主要功能就是定义别名，定义别名的目的就是为了方便访问节点。不过一般会在节点命名的时候会加上 label，然后通过&label 来访问节点
```c
aliases {
    csi2dphy0 = &csi2_dphy0;
    csi2dphy1 = &csi2_dphy1;
    csi2dphy2 = &csi2_dphy2;
    dsi0 = &dsi0;
    dsi1 = &dsi1;
    ethernet0 = &gmac0;
    ethernet1 = &gmac1;
    gpio0 = &gpio0;
    gpio1 = &gpio1;
    gpio2 = &gpio2;
    gpio3 = &gpio3;
    gpio4 = &gpio4;
    i2c0 = &i2c0;
    i2c1 = &i2c1;
    i2c2 = &i2c2;
    i2c3 = &i2c3;
    i2c4 = &i2c4;
    i2c5 = &i2c5;
    mmc0 = &sdhci;
    mmc1 = &sdmmc0;
    mmc2 = &sdmmc1;
    mmc3 = &sdmmc2;
    serial0 = &uart0;
    serial1 = &uart1;
    serial2 = &uart2;
    serial3 = &uart3;
    serial4 = &uart4;
    serial5 = &uart5;
    serial6 = &uart6;
    serial7 = &uart7;
    serial8 = &uart8;
    serial9 = &uart9;
    spi0 = &spi0;
    spi1 = &spi1;
    spi2 = &spi2;
    spi3 = &spi3;
    spi4 = &sfc; // for U-Boot
};
```
### chosen子节点
## 绑定文档信息
设备树是用来描述板子上的设备信息的，不同的设备其信息不同，反映到设备树中就是属性不同，我们可以在`kernel/Documentation/devicetree/bindings`查阅
## 设备树常用的OF操作函数
Linux 内核给我们提供了一系列的函数来获取设备树中的节点或者属性信息，这一系列的函数都有一个统一的前缀“of_”，所以在很多资料里面也被叫做 OF 函数。这些 OF 函数原型都定义在 `include/linux/of.h` 文件中
### 查找节点的OF函数
设备都是以节点的形式挂载设备书上，要想获取这个设备的星系必须先获取到这个设备的节点，在Linux中使用`device_node`结构体来描述一个节点
```c
// include/linux/of.h
struct device_node {
	const char *name;
	const char *type;
	phandle phandle;
	const char *full_name;
	struct fwnode_handle fwnode;

	struct	property *properties;
	struct	property *deadprops;	/* removed properties */
	struct	device_node *parent;
	struct	device_node *child;
	struct	device_node *sibling;
#if defined(CONFIG_OF_KOBJ)
	struct	kobject kobj;
#endif
	unsigned long _flags;
	void	*data;
#if defined(CONFIG_SPARC)
	const char *path_component_name;
	unsigned int unique_id;
	struct of_irq_controller *irq_trans;
#endif
};
```
下面5个OF查找节点函数
1. `of_find_node_by_name`函数，通过节点名字查找指定的节点
    ```c    
    struct device_node *of_find_node_by_name(struct device_node *from, const char *name);
    ```
    - `from` 开始查找的节点，如果为NULL表示从根节点开始查找整个设备树
    - `name` 要查找的节点名字
    返回找到的节点，如果为NULl表示查找失败
2. `of_find_node_by_type`函数，通过`device_type`属性查找指定的节点
    ```c
    struct device_node *of_find_node_by_type(struct device_node *from, const char *type);
    ```
    - `from` 开始查找的节点，如果为NULL表示从根节点开始查找整个设备树
    - `type` 要查找的节点对应的`type`字符串，也就是`device_type`属性值
    返回找到的节点，如果为NULl表示查找失败
3. `of_find_compatible_node`函数，根据`device_type`和`compatible`这两个属性查找指定节点
    ```c
    struct device_node *of_find_compatible_node(struct device_node *from, const char *type， const char *compat);
    ```
    - `from` 开始查找的节点，如果为NULL表示从根节点开始查找整个设备树
    - `type` 要查找的节点对应的`type`字符串，也就是`device_type`属性值，可以为NULL，表示忽略该属性
    - `compat` 要查找节点所对应的`compatible`属性列表
    返回找到的节点，如果为NULl表示查找失败
4. `of_find_matching_node_and_match`函数，通过`of_device_id` 匹配表来查找指定的节点
    ```c
    struct device_node *of_find_matching_node_and_match(struct device_node *from, const struct of_device_id *matches, const struct of_device_id **match);
    ```
    - `from` 开始查找的节点，如果为NULL表示从根节点开始查找整个设备树
    - `matches` `of_device_id`匹配表，也就是再次匹配表里面查找节点
    - `match` 找到的匹配的`of_device_id`
    返回找到的节点，如果为NULl表示查找失败
5. `of_find_node_by_path`函数，通过路径来查找指定的节点
    - `path` 带有全路径的节点名，可以使用节点别名
    返回找到的节点，如果为NULl表示查找失败
### 查找父子节点的OF函数
1. `of_get_parent`函数，用于获取指定节点的父节点
    ```c
    struct device_node *of_get_parent(const struct device_node *node);
    ```
    - `node` 要查找的父节点的节点
    返回值为找到的父亲节点
2.  `of_get_next_child`函数，用于获取指定节点的子节点
    ```c
    struct device_node *of_get_next_child(const struct device_node *node, struct device_node *prev);
    ```
    - `node` 父节点
    - `prev` 前一个子节点
    返回值为找到的下一个子节点
### 提取属性值的OF函数
节点的属性信息里面保存了驱动所需要的内容，Linux 内核中使用结构体 `property` 表示属性
```c
struct property {
	char	*name;
	int	length;
	void	*value;
	struct property *next;
#if defined(CONFIG_OF_DYNAMIC) || defined(CONFIG_SPARC)
	unsigned long _flags;
#endif
#if defined(CONFIG_OF_PROMTREE)
	unsigned int unique_id;
#endif
#if defined(CONFIG_OF_KOBJ)
	struct bin_attribute attr;
#endif
};
```
1. `of_find_property` 函数，用于查找指定的属性
    ```c
    struct property *of_find_property(const struct device_node *np, const char *name, int *lenp);
    ```
    - `np` 设备节点
    - `name` 属性名字
    - `lenp` 属性值的字节数
    返回值为找到的属性
2. `of_property_count_elems_of_size`函数，用于获取属性中元素的数量
    ```c
    int of_property_count_elems_of_size(const struct device_node *np, const char *propname, int elem_size);
    ```
    - `np` 设备节点
    - `proname` 需要统计元素数量的属性名字
    - `elem_size` 元素长度
    返回值为得到的属性元素数量
3. `of_property_read_u32_index`函数，用于从属性中获取指定标号的 `u32` 类型数据值
    ```c
    int of_property_read_u32_index(const struct device_node *np, const char *propname, u32 index, u32 *out_value);
    ```
    - `np` 设备节点
    - `proname` 需要统计元素数量的属性名字
    - `index` 要读取的值标号
    - `out_value` 读取到的值
    返回值：`0` 读取成功，负值，读取失败，`-EINVAL` 表示属性不存在，`-ENODATA` 表示没有
要读取的数据，`-EOVERFLOW` 表示属性值列表太小
4. `of_property_read_u8_array`函数，  
    `of_property_read_u16_array`函数，
    `of_property_read_u32_array`函数，
    `of_property_read_u64_array`函数，这些函数分别是读取属性中 u8、u16、u32 和 u64 类型的数组数据
    ```c
    int of_property_read_u8_array(const struct device_node *np, const char *propname, u8 *out_values, size_t sz);
    int of_property_read_u16_array(const struct device_node *np, const char *propname, u16 *out_values, size_t sz);
    int of_property_read_u32_array(const struct device_node *np, const char *propname, u32 *out_values, size_t sz);
    int of_property_read_u64_array(const struct device_node *np, const char *propname, u64 *out_values, size_t sz);
    ```
    - `np` 设备节点
    - `proname` 要读取的属性名字
    - `out_value` 读取到的数组值
    返回值：`0` 读取成功，负值，读取失败，`-EINVAL` 表示属性不存在，`-ENODATA` 表示没有
要读取的数据，`-EOVERFLOW` 表示属性值列表太小
5. `of_property_read_u8`函数，  
    `of_property_read_u16`函数，
    `of_property_read_u32`函数，
    `of_property_read_u64`函数，这些函数分别是读取属性中 u8、u16、u32 和 u64 类型属性值
    ```c
    int of_property_read_u8_array(const struct device_node *np, const char *propname, u8 *out_values);
    int of_property_read_u16_array(const struct device_node *np, const char *propname, u16 *out_values);
    int of_property_read_u32_array(const struct device_node *np, const char *propname, u32 *out_values);
    int of_property_read_u64_array(const struct device_node *np, const char *propname, u64 *out_values);
    ```
    - `np` 设备节点
    - `proname` 要读取的属性名字
    - `out_value` 读取到的数组值
    返回值：`0` 读取成功，负值，读取失败，`-EINVAL` 表示属性不存在，`-ENODATA` 表示没有
要读取的数据，`-EOVERFLOW` 表示属性值列表太小
6. `of_property_read_string`函数，用于读取属性中字符串值
    ```c
    int of_property_read_string(const struct device_node *np, const char *propname, const char **out_string);
    ```
    - `np` 设备节点
    - `proname` 读取到的属性名字
    - `out_string` 读取到的字符串值
    返回值：0，读取成功，负值，读取失败
7. `of_n_addr_cells`函数，用于获取`#address-cells` 属性值
    ```c
    int of_n_addr_cells(struct device_node *np)
    ```
    - `np` 设备节点
    返回值：获取到的`#address-cells`属性值
8. `of_n_size_cells`函数，用于获取`#size-cells` 属性值
    ```c
    int of_n_size_cells(struct device_node *np)
    ```
    - `np` 设备节点
    返回值：获取到的`#address-cells`属性值
### 其他常用OF函数
1. `of_device_is_compatible`函数用于查看节点的 `compatible` 属性是否有包含 `name` 指定的字符
串，也就是检查设备节点的兼容性
    ```c
    int of_device_is_compatible(const struct device_node *device, const char *name);
    ```
    - ``
    返回值
2. `of_get_address`函数用于获取地址相关属性，主要是`reg`或者`assigned-addresses`属性值
    ```c
    const __be32 *of_get_address(struct device_node *dev, int index, u64 *size, unsigned int *flags);
    ```
    - `dev` 设备节点
    - `index` 要读取的地址标号
    - `size` 地址长度
    - `flags` 参数 
    返回值：读取到的地址数据首地址，为 NULL 的话表示读取失败
3. `of_translate_address`函数用于将从设备树读取到的物理地址转换为虚拟地址
    ```c
    u64 of_translate_address(struct device_node *np, const __be32 *addr)；
    ```
    - `dev` 设备节点
    - `in_addr` 要转换的地址
    返回值：得到的物理地址，如果为 `OF_BAD_ADDR` 的话表示转换失败
4. `of_address_to_resource`函数用于
    ```c
    int of_address_to_resource(struct device_node *dev, int index, struct resource *r);
    ```
    - `dev` 设备节点
    - `index` 地址资源标号
    - `r` 得到的`resource`类型资源的值
    返回值：0，成功；负值，失败
5. `of_iomap`函数，用于直接内存映射
    ```c
    void __iomem *of_iomap(struct device_node *device, int index);
    ```
    - `np` 设备节点
    - `index` `reg` 属性中要完成内存映射的段，如果 `reg` 属性只有一段的话 `index` 就设置为 0。 
    返回值：经过内存映射后的虚拟内存首地址，如果为 NULL 的话表示内存映射失败。
              