# pinctrl和gpio子系统
Linux 驱动讲究驱动分离与分层，pinctrl 和 gpio 子系统就是驱动分离与分层思想下的产物
## pinctrl子系统
该子系统的重点是设置PIN的复用和电气属性
### pinctrl子系统简介
Pinctrl子系统的主要工作内容
1. 获取设备树中pin信息
2. 根据获取到的pin信息来设置pin的复用功能
3. 根据获取到的pin信息来设置pin的电气特性
### rk3568的pinctrl子系统驱动
- PIN配置信息详解
要使用pinctrl子系统，需要在设备树里面配置PIN的配置信息，一般会在设备树里面创建一个节点来描述PIN的配置信息，以`rk3568.dtsi`中的pinctrl节点为例
```dts
pinctrl: pinctrl {
		compatible = "rockchip,rk3568-pinctrl";
		rockchip,grf = <&grf>;
		rockchip,pmu = <&pmugrf>;
		#address-cells = <2>;
		#size-cells = <2>;
		ranges;

		gpio0: gpio@fdd60000 {
			compatible = "rockchip,gpio-bank";
			reg = <0x0 0xfdd60000 0x0 0x100>;
			interrupts = <GIC_SPI 33 IRQ_TYPE_LEVEL_HIGH>;
			clocks = <&pmucru PCLK_GPIO0>, <&pmucru DBCLK_GPIO0>;

			gpio-controller;
			#gpio-cells = <2>;
			gpio-ranges = <&pinctrl 0 0 32>;
			interrupt-controller;
			#interrupt-cells = <2>;
		};
    ......
    };
```
rk3568有五组gpio，每组对应的就寄存器地址不同，基地址不同，具体IO的地址可以通过基地址+偏移的方式计算获取，而pin具体的配置在`rk3568-pinctrl.dtsi`这个文件中，这个文件由编译内核后生成，不能修改这个文件，具体看文件
每个pinctrl节点必须至少包含一个子节点来存放pinctrl，也叫pinctrl集，根据`rockchip,pinctrl.txt`里介绍，引脚复用设置的格式为
```
rockchip,pins=<PIN_BANK PIN_BANK_IDX MUX &phandle>
```
- **PIN_BANK**
	`PIN_BANK`是PIN所属的组，RK3568一共5组PIN：GPIO0~GPIO4
- **PIN_BANK_IDX**
	`PIN_BANK_IDX`是组内编号，在`include/dt-bindings/pinctrl/rockchip.h`有定义
- **MUX**
	`MUX`就是设置PIN的复用功能，一个PIN最多有16个复用功能，也在`include/dt-bindings/pinctrl/rockchip.h`中有定义
- **phandle**
	`phandle`用来描述一些引脚的通用配置信息，在`scripts/dtc/include-prefixes/arm/rockchip-pinconf.dtsi`中有描述
### 在设备树里添加pinctrl节点模板
假设我们需要将`GPIO0_D1`这个PIN服用为`UART2_TX`引脚，如下为pinctrl添加过程
- **创建对应节点**
在pinctrl节点下添加对应的`uart2`子节点，然后再子节点里创建一个`uart2m0_xfer:uart2m0-xfer`子节点
	```
	&pinctrl{
		uart2P{
			/omit-if-no-ref/
			uart2m0_xfer: uart2m0-xfer{

			};
		};
	};
	```
- 添加`rockchip,pins`属性
这个属性是真正用来描述PIN配置信息的
	```
	&pinctrl{
		uart2{
			/omit-if-no-ref/
			uart2m0_xfer: uart2m0-xfer{
				rockchip,pins = <0 RK_PD1 1 &pcfg_pull_up>;
			};
		};
	};
	```
## gpio子系统
该子系统同于初始化GPIO并提供相关的API函数，方便开发者使用GPIO
### rk3568的gpio子系统驱动
- **设备树中gpio信息**
	```dts 
	gpio0: gpio@fdd60000 {
		compatible = "rockchip,gpio-bank";
		reg = <0x0 0xfdd60000 0x0 0x100>;
		interrupts = <GIC_SPI 33 IRQ_TYPE_LEVEL_HIGH>;
		clocks = <&pmucru PCLK_GPIO0>, <&pmucru DBCLK_GPIO0>;

		gpio-controller;
		#gpio-cells = <2>;
		gpio-ranges = <&pinctrl 0 0 32>;
		interrupt-controller;
		#interrupt-cells = <2>;
	};
	``` 
	- `compatible`属性值为`rockchip,gpio-bank`，在内核中搜索该字符串就可以找到对应的GPIO驱动文件
	- `reg`属性设置了GPIO0控制器的寄存器基地址
	- `interrupts`属性描述GPIO0控制器对应的中断信息
	- `clocks`属性指定这个GPIO0控制器的时钟
	- `gpio-controller`表示gpio0这个节点是个GPIO控制器，每个GPIO控制器节点必须包含`gpio-controller`属性
	- `#gpio-cells`属性定义引用此 GPIO 控制器时所需的参数数量

	这是GPIO0控制器节点的内容，使用某个具体引脚作为GPIO时还要进一步设置，比如
	```dts
	reset-gpios = <&gpio3 PK_PB6 GPIO_ACTIVE_LOW>;
	```
	- 属性`reset-gpios`描述了复位脚用了哪个IO
	- 属性值`&gpio3`表示复位脚使用IO属于GPIO3这一组
	- 属性值`PK_PB6`表示使用的时GPIO3_B6这一个引脚
	- 属性值`GPIO_ACTIVE_LOW`表示复位低电平有效

	第三个属性值在`include/linux/gpio/machine.h`中定义了`gpio_lookup_flags`的枚举变量
	```c
	enum gpio_lookup_flags {
	GPIO_ACTIVE_HIGH = (0 << 0),
	GPIO_ACTIVE_LOW = (1 << 0),
	GPIO_OPEN_DRAIN = (1 << 1),
	GPIO_OPEN_SOURCE = (1 << 2),
	GPIO_PERSISTENT = (0 << 3),
	GPIO_TRANSITORY = (1 << 3),
	};
	``` 
### gpio子系统API函数
1. `gpio_request`函数
	用于申请一个GPIO引脚，函数原型如下
	```C
	static inline int gpio_request(unsigned gpio, const char *label);
	``` 
	- `gpio` 要申请的gpio标号，使用`of_get_named_gpio`函数从设备树获取指定GPIO属性信息
	- `label` 给这个GPIO设置名字
	返回值：0，成功，其他值失败
2. `gpio_free`函数
	用于释放IO，函数原型如下
	```C
	static inline void gpio_free(unsigned gpio);
	```
	- `gpio` 要释放的gpio标号
	返回值：无
3. `gpio_direction_input`函数
	用于设置某个GPIO输入，函数原型如下
	```c
	static inline int gpio_direction_input(unsigned gpio);
	```
	- `gpio` 要设置输入的gpio标号
	返回值：0，成功，其他值失败
4. `gpio_direction_output`函数
	用于设置某个GPIO输出，函数原型如下
	```c
	static inline int gpio_direction_output(unsigned gpio, int value);
	```
	- `gpio` 要设置输出的gpio标号
	- `value` 要输出的值
	返回值：0，成功，其他值失败
5. `gpio_get_value`函数
	用于获取某个 GPIO 的值(0 或 1)，函数原型如下
	```c
	static inline int gpio_get_value(unsigned gpio);
	```
	- `gpio` 要设置输入的gpio标号
	返回值：0，成功，其他值失败
6. `gpio_set_value`函数
	此函数用于设置某个 GPIO 的值，函数原型如下
	```c
	static inline void gpio_set_value(unsigned gpio, int value);
	```
	- `gpio` 要设置输出的gpio标号
	- `value` 要输出的值
	返回值：0，成功，其他值失败
### 设备树中添加gpio节点模板
1. 创建LED设备节点，在根节点`/`下创建设备节点
2. 添加GPIO属性信息
```dts
gpioled{
	compatible = "alientek,led";
	led-gpio = <&gpio0 RK_PC0 GPIO_ACTIVE_HIGH>
	status = "okay";
}; 
```
### 与gpio相关的of函数
1. `of_gpio_named_count`函数
	用于获取设备树节点中指定名称的GPIO属性的GPIO数量，函数原型如下
	```c
	static inline int of_gpio_named_count(struct device_node *np, const char* propname);
	```
	- `np` 设备节点
	- `propname` 要统计的GPIO属性
	返回值：正值，统计到的GPIO数量，负值，失败
2. `of_get_named_gpio`函数
	用于获取设备树节点中默认GPIO属性（名为 "gpios"）的GPIO数量
	```c
	static inline int of_gpio_count(struct device_node *np);
	```
	- `np` 设备节点
	返回值：正值，统计到的GPIO数量，负值，失败
## 程序编写
### 修改设备树文件
在设备树文件的`/`根节点下面创建设备节点
```
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
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm-generic/unistd.h>

#define DTSLED_CNT       1
#define DTSLED_NAME      "gpioled"

#define LEDOFF  0
#define LEDON   1

#define PMU_GRF_BASE                (0xfd520000)
/* 引脚复用功能 */
#define PMU_GRF_GPIO0C_IOMUX_L      (PMU_GRF_BASE + 0x0010)
/* 引脚驱动能力 */
#define PMU_GRF_GPIO0C_DS_0         (PMU_GRF_BASE + 0x0090)

#define GPIO0_BASE                  (0xfdd60000)
/* 引脚高低电平 */
#define GPIO_SWPORT_DR_H            (GPIO0_BASE + 0x0004)
/* 引脚输入输出 */
#define GPIO_SWPORT_DDR_H           (GPIO0_BASE + 0x000c)

static void __iomem* PMU_GRF_GPIO0C_IOMUX_L_PI;
static void __iomem* PMU_GRF_GPIO0C_DS_0_PI;
static void __iomem* GPIO_SWPORT_DR_H_PI;
static void __iomem* GPIO_SWPORT_DDR_H_PI;

struct gpioled_dev
{
    dev_t devid;
    int major;
    int minor;
    struct cdev cdev;
    struct class* class;
    struct device* device;
    struct device_node* nd;
    int led_gpio;
};

struct gpioled_dev gpioled;

void led_switch(u8 sta)
{
    u32 val = 0;
    if (sta == LEDOFF)
    {
        val = readl(GPIO_SWPORT_DR_H_PI);
        /* bit0 清零 */
        val &= ~(0x1 << 0);
        /* bit16 置1，允许写bit0 低电平 */
        val |= ((0x1 << 16) | (0x0 << 0));
        writel(val, GPIO_SWPORT_DR_H_PI);
    }
    else if (sta == LEDON)
    {
        val = readl(GPIO_SWPORT_DR_H_PI);
        /* bit0 清零 */
        val &= ~(0x1 << 0);
        /* bit16 置1，允许写bit0 高电平 */
        val |= ((0x1 << 16) | (0x1 << 1));
        writel(val, GPIO_SWPORT_DR_H_PI);
    }
}

void led_remap(void)
{
    PMU_GRF_GPIO0C_IOMUX_L_PI = of_iomap(gpioled.nd, 0);
    PMU_GRF_GPIO0C_DS_0_PI = of_iomap(gpioled.nd, 1);
    GPIO_SWPORT_DR_H_PI = of_iomap(gpioled.nd, 2);
    GPIO_SWPORT_DDR_H_PI = of_iomap(gpioled.nd, 3);
}

void led_unmap(void)
{
    iounmap(PMU_GRF_GPIO0C_IOMUX_L_PI);
    iounmap(PMU_GRF_GPIO0C_DS_0_PI);
    iounmap(GPIO_SWPORT_DR_H_PI);
    iounmap(GPIO_SWPORT_DDR_H_PI);
}

/* 打开设备 */
static int led_open(struct inode* inode, struct file* filp)
{
    /* 用户实现具体功能 */
    filp->private_data = &gpioled;
    return 0;
}

/* 从设备读取 */
static ssize_t led_read(struct file* filp, char __user* buf, size_t cnt, loff_t* offt)
{
    return 0;
}

/* 向设备写数据 */
static ssize_t led_write(struct file* filp, const char __user* buf, size_t cnt, loff_t* offt)
{
    int ret_value = 0;
    unsigned char databuf[1];
    unsigned char ledstat;
    ret_value = copy_from_user(databuf, buf, cnt);
    if (ret_value < 0)
    {
        printk(KERN_ERR "led: kernel write failed");
        return -EFAULT;
    }
    ledstat = databuf[0];
    if (ledstat == LEDOFF)
    {
        led_switch(LEDOFF);
    }
    else
    {
        led_switch(LEDON);
    }

    return 0;
}

/* 关闭/释放设备 */
static int led_release(struct inode* inode, struct file* filp)
{
    /* 用户实现具体功能 */
    return 0;
}

static struct file_operations gpioled_fops = {
    .owner = THIS_MODULE,
    .open = led_open,
    .read = led_read,
    .write = led_write,
    .release = led_release,
};
/* 驱动入口函数 */
static int __init led_init(void)
{
    int ret_value = 0;
    const char* str;
    struct property* proper;

    /* 获取设备树中的属性数据 */
    /* 1、获取设备节点 */
    gpioled.nd = of_find_node_by_pah("/rk3568_led");
    if (gpioled.nd == NULL)t
    {
        printk("rk3568_led node not find\r\n");
        goto fail_find_node;
    }
    else
    {
        printk("rk3568_led node find\r\n");
    }

    /* 2、获取compatible属性内容 */
    proper = of_find_property(gpioled.nd, "compatible", NULL);
    if (proper == NULL)
    {
        printk("compatible property find dailed\r\n");
    }
    else
    {
        printk("compatible = %s\r\n", (char*)proper->value);
    }

    /* 3、获取status属性内容 */
    ret_value = of_property_read_string(gpioled.nd, "status", &str);
    if (ret_value < 0)
    {
        printk("status read failed \r\n");
    }
    else
    {
        printk("status = %s\r\n", str);
    }
    
    /* 4、获取设备树中的gpio属性，得到LED所使用的LED编号 */
    gpioled.led_gpio = of_get_named_gpio(gpioled.nd, "led-gpio", 0);
    if (gpioled.led_gpio < 0)
    {
        printk("can't get led-gpio\r\n");
        return -EINVAL;
    }
    printk("led-gpio num = %d\r\n", gpioled.led_gpio);
    /* 5、向gpio子系统申请使用GPIO */
    ret_value = gpio_request(gpioled.led_gpio, "LED-GPIO");
    if (ret_value)
    {
        printk(KERN_ERR "gpioled: Failed to request led-gpio\n");
        return ret_value;
    }

    /* 6、设置GPIO为输出，并且输出低电平，默认关闭LED灯 */
    ret_value = gpio_direction_output(gpioled.led_gpio, 0);
    if (ret_value < 0)
    {
        printk(KERN_ERR "gpioled: Failed to set led-gpio\n");
    }

    /* 静态申请设备 */
    if (gpioled.major)
    {
        gpioled.devid = MKDEV(gpioled.major, 0);
        ret_value = register_chrdev_region(gpioled.devid, DTSLED_CNT, DTSLED_NAME);
        if (ret_value < 0)
        {
            pr_err("cannot register %s char driver [ret_value = %d]\n", DTSLED_NAME, DTSLED_CNT);
            goto fail_map;
        }
    }
    /* 动态申请设备 */
    else
    {
        ret_value = alloc_chrdev_region(&gpioled.devid, 0, DTSLED_CNT, DTSLED_NAME);
        if (ret_value < 0)
        {
            pr_err("cannot register %s char driver [ret_value = %d]\n", DTSLED_NAME, DTSLED_CNT);
            goto fail_map;
        }
        gpioled.major = MAJOR(gpioled.devid);
        gpioled.minor = MINOR(gpioled.devid);
    }

    printk("gpioled major = %d, minor = %d\r\n", gpioled.major, gpioled.minor);
    /* 自动创建设备节点 */
    gpioled.cdev.owner = THIS_MODULE;
    /* 初始化字符设备结构 */
    cdev_init(&gpioled.cdev, &gpioled_fops);
    /* 添加字符设备 */
    ret_value = cdev_add(&gpioled.cdev, gpioled.devid, DTSLED_CNT);
    if (ret_value < 0)
    {
        goto del_unregister;
    }
    /* 创建类 */
    gpioled.class = class_create(THIS_MODULE, DTSLED_NAME);
    if (IS_ERR(gpioled.class))
    {
        goto del_cdev;
    }
    /* 创建设备 */
    gpioled.device = device_create(gpioled.class, NULL, gpioled.devid, NULL, DTSLED_NAME);
        if (IS_ERR(gpioled.device))
    {
        goto destroy_class;
    }

    return 0;

destroy_class:
    class_destroy(gpioled.class);
del_cdev:
    cdev_del(&gpioled.cdev);
del_unregister:
    unregister_chrdev_region(gpioled.devid, DTSLED_CNT);
fail_map:
    led_unmap();
fail_find_node:
    return -EIO;
}

/* 驱动出口函数 */
static void __exit led_exit(void)
{
    /* 注销字符设备驱动 */
    cdev_del(&gpioled.cdev);
    unregister_chrdev_region(gpioled.devid, DTSLED_CNT);

    device_destroy(gpioled.class, DTSLED_CNT);
    class_destroy(gpioled.class);

    printk(KERN_INFO "led: module exit successfully\r\n");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("qiu");
MODULE_INFO(intree, "Y");
```