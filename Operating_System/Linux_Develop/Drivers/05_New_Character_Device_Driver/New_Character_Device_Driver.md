
#  新字符设备驱动
##  新字符设备驱动原理
### 分配和释放设备号
在之前使用`register_chrdev`函数注册字符设备的时候只需要给定一个主设备号即可，但是这样会带来两个问题：
1. 需要我们事先确定好哪些主设备号没有使用
2. 会将一个主设备号下的所有次设备号都使用掉      

解决方法就是在使用设备号时想Linux内核申请
- 如果没有指定设备号的话就使用`alloc_chrdev_region`函数来申请设备号
`int alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count, const char *name) `
    - `dev`，保存申请到的设备号
    - `baseminor`，次设备号起始地址
    - `count`，要申请的设备号数量
    - `name`，设备名字    
- 如果给定了设备的主设备号和次设备号就使用``函数来注册设备号即可
`int register_chrdev_region(dev_t from, unsigned count, const char *name) `
    - `from` 是要申请的起始设备号即给定的
    - `count` 是要申请的数量
    - `name` 是设备的名字
- 如果要注销设备号的话，可以使用`unregister_chrdev_region`
`void unregister_chrdev_region(dev_t from, unsigned count)`


大部分驱动次设备号都选择0
### 新字符设备注册方法 
1. 字符设备结构，在Linux中使用`cdev`结构体表示一个字符设备
    ```c
    // kernel/include/linux/cdev.h
    struct cdev {
        struct kobject kobj;
        struct module *owner;
        const struct file_operations *ops;              /* 文件操作符 */
        struct list_head list;                          
        dev_t dev;                                      /* 设备号 */
        unsigned int count;
    } __randomize_layout;
    ```
2. 初始化字符设备结构，使用`cdev_init`    
    ```void cdev_init(struct cdev *cdev, const struct file_operations *fops)```
    - `cdev` 是要初始化的`cdev`结构体变量
    - `fops` 是字符设备文件操作函数集合

3. `cdev_add` 函数用于向Linux系统添加字符设备结构
    ```int cdev_add(struct cdev *p, dev_t dev, unsigned count)```
    - `p` 是只想要添加的字符设备
    - `dev` 是设备所使用的设备号
    - `count` 是要添加的设备数量
4. 卸载驱动的时候一定要使用`cdev_del`函数从Linux内核中删除相应的字符设备
    ```void cdev_del(struct cdev *p)```
    - `p` 是只想要删除的字符设备
### 示例
```c
int major;                  /* 主设备号       */ 
int minor;                  /* 次设备号      */
dev_t devid;                /* 设备号        */

if(major)
{
    devid = MKDEV(major, 0);
    register_chrdev_region(devid, 1, "test");
}
else
{
    alloc_chrdev_region(&devid, 0, 1, "test");
    major = MAJOR(devid);   /* 获取分配的主设备号 */
    minor = MINOR(devid);   /* 获取分配的次设备号 */
}

/* 定义一个cdev的结构体变量表示一个字符设备 */
struct cdev test_dev;
/* 设备操作函数 */ 
static struct file_operations test_fops = {
    .owner = THIS_MODULE, 
};

testcdev.owner = THIS_MODULE;
cdev_init(&testcdev, &test_fops);
cdev_add($testcdev, devid, 1);
cdev_del(&testdev);
```
## 自动创建设备节点
之前在挂载完驱动程序后还需要手动创建设备节点，有点麻烦，可以在驱动中实现自动创建设备节点，自动创建设备节点的工作是在驱动程序的入口函数中完成的
### `mdev`机制
udev 是一个用户程序，在Linux下通过udev来实现设备文件的创建与删除，udev可以检测系统中硬件设备状态，可以根据系统中硬件设备状态来创建或者删除设备文件。
### 创建和删除类 
自动创建设备节点的工作是在驱动程序的入口函数中完成的，一般在`cdev_add`函数后面添加自动创建设备节点相关代码，首先创建一个类
```c
// kernel/include/linux/device.h
extern struct class * __must_check __class_create(struct module *owner,
						  const char *name,
						  struct lock_class_key *key);
extern void class_destroy(struct class *cls);

/* This is a #define to keep the compiler from merging different
 * instances of the __key variable */
#define class_create(owner, name)		\
({						\
	static struct lock_class_key __key;	\
	__class_create(owner, name, &__key);	\
})
```
展开后函数原型为
```struct class *class_create(struct module *owner, const char *name)```
- `owner` 一般为`THIS_MODULE`
- `name` 是类名
返回指向结构体class的指针，即创建的类    

卸载驱动程序的时候需要删除类，原型如下
```void class_destroy(struct class *cls)```
- `cls` 是要删除的类
### 创建和删除设备
节创建好类以后还不能实现自动创建设备节点，我们还需要在这个类下创建一个设备。使用`device_create`函数在类下面创建设备
```c
struct device *device_create(struct class *class, struct device *parent,
			     dev_t devt, void *drvdata,
			     const char *fmt, ...);
```
- `class` 是设备要到创建的哪个类下面
- `parent` 一般没有父设备所以一般为`NULL`
- `devt` 设备号
- `drvdata` 是设备可能会使用的数据，一般也为`NULL`
- `fmt` 是设备名字，如`fmt=xxx`的话，就会生产`/dev/xxx`这个设备文件
返回值为创建好的设备

同样的，卸载驱动的时候需要删除掉创建的设备
```c
extern void device_destroy(struct class *class, dev_t devt);
``` 
- `class` 是要删除的类
- `devt` 是要删除的设备
### 参考示例
```c
struct class *class;
struct device *device;
dev_t devid;

static int __init xxx_init(void)
{
    class = class_create(THIS_MODULE, "xxx");
    device = device_create(class, NULL, devid, NULL, "xxx");
    return 0;
}

static void __exit led_exit(void)
{
    device_destroy(newchrled.class, newchrled.devicd);
    class_destroy(newchrled.class);
}

module_init(led_init);
module_exit(led_exit);
```
## 设置文件私有数据
对于一个设备的所有属性信息我们最好将其做成一个结构体
```c
struct test_dev{
    struct cdev cdev;
    struct class *class;
    struct device *device;
    dev_t devid;
    int major;
    int minor;
}

struct test_dev testdev;

static int test_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &testdev;
    return 0;
}
```
## 程序编写
### 驱动程序编写
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
#include <asm/uaccess.h>
#include <asm/io.h>
#include <asm-generic/unistd.h>

#define NEWCHRLED_CNT       1
#define NEWCHRLED_NAME      "newchrled"

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

struct newchrled_dev
{
    dev_t devid;
    int major;
    int minor;
    struct cdev cdev;
    struct class* class;
    struct device* device;
};

struct newchrled_dev newchrled;

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
    PMU_GRF_GPIO0C_IOMUX_L_PI = ioremap(PMU_GRF_GPIO0C_IOMUX_L, 4);
    PMU_GRF_GPIO0C_DS_0_PI = ioremap(PMU_GRF_GPIO0C_DS_0, 4);
    GPIO_SWPORT_DR_H_PI = ioremap(GPIO_SWPORT_DR_H, 4);
    GPIO_SWPORT_DDR_H_PI = ioremap(GPIO_SWPORT_DDR_H, 4);
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
    filp->private_data = &newchrled;
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

static struct file_operations newchrled_fops = {
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
    u32 val = 0;
    /* 初始化 LED */
    /* 1、寄存器地址映射 */
    led_remap();
    /* 2、设置GPIO0_C0为GPIO功能 */
    val = readl(PMU_GRF_GPIO0C_IOMUX_L_PI);
    val &= ~(0x7 << 0);
    val |= ((0x7 << 16) | (0x0 << 0));
    writel(val, PMU_GRF_GPIO0C_IOMUX_L_PI);
    /* 3、设置GPIO0_C0驱动能力为level5 */
    val = readl(PMU_GRF_GPIO0C_DS_0_PI);
    val &= ~(0x3f << 0);
    val |= ((0x3f << 16) | (0x3f << 0));
    writel(val, PMU_GRF_GPIO0C_DS_0_PI);
    /* 4、设置 GPIO0_C0 为输出 */
    val = readl(GPIO_SWPORT_DDR_H_PI);
    val &= ~(0x1 << 0);
    val |= ((0x1 << 16) | (0x1 << 0));
    writel(val, GPIO_SWPORT_DDR_H_PI);
    /* 5、设置 GPIO0_C0 为低电平，关闭 LED 灯。*/
    val = readl(GPIO_SWPORT_DR_H_PI);
    val &= ~(0x1 << 0);
    val |= ((0x1 << 16) | (0x0 << 0));
    writel(val, GPIO_SWPORT_DR_H_PI);

    if (newchrled.major)
    {
        newchrled.devid = MKDEV(newchrled.major, 0);
        ret_value = register_chrdev_region(newchrled.devid, NEWCHRLED_CNT, NEWCHRLED_NAME);
        if (ret_value < 0)
        {
            pr_err("cannot register %s char driver [ret = %d]\n", NEWCHRLED_NAME, NEWCHRLED_CNT);
            goto fail_map;
        }
    }
    else
    {
        ret_value = alloc_chrdev_region(&newchrled.devid, 0, NEWCHRLED_CNT, NEWCHRLED_NAME);
        if (ret_value < 0)
        {
            pr_err("cannot register %s char driver [ret = %d]\n", NEWCHRLED_NAME, NEWCHRLED_CNT);
            goto fail_map;
        }
        newchrled.major = MAJOR(newchrled.devid);
        newchrled.minor = MINOR(newchrled.devid);
    }

    printk("newchrled major = %d, minor = %d\r\n", newchrled.major, newchrled.minor);

    newchrled.cdev.owner = THIS_MODULE;
    cdev_init(&newchrled.cdev, &newchrled_fops);

    ret_value = cdev_add(&newchrled.cdev, newchrled.devid, NEWCHRLED_CNT);
    if (ret_value < 0)
    {
        goto del_unregister;
    }

    newchrled.class = class_create(THIS_MODULE, NEWCHRLED_NAME);
    if (IS_ERR(newchrled.class))
    {
        goto del_cdev;
    }

    newchrled.device = device_create(newchrled.class, NULL, newchrled.devid, NULL, NEWCHRLED_NAME);
        if (IS_ERR(newchrled.device))
    {
        goto destroy_class;
    }

    return 0;

destroy_class:
    class_destroy(newchrled.class);
del_cdev:
    cdev_del(&newchrled.cdev);
del_unregister:
    unregister_chrdev_region(newchrled.devid, NEWCHRLED_CNT);
fail_map:
    led_unmap();
    return -EIO;
}

/* 驱动出口函数 */
static void __exit led_exit(void)
{
    /* 取消映射 */
    led_unmap();
    /* 注销字符设备驱动 */
    cdev_del(&newchrled.cdev);
    unregister_chrdev_region(newchrled.devid, NEWCHRLED_CNT);

    device_destroy(newchrled.class, NEWCHRLED_CNT);
    class_destroy(newchrled.class);

    printk(KERN_INFO "led: module exit successfully\r\n");
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("qiu");
MODULE_INFO(intree, "Y");
```
### 应用程序编写
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define LEDOFF  0
#define LEDON   1

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Error Usage!\r\n");
        return -1;
    }
    char* filename = argv[1];
    int fd = open(filename, O_RDWR);
    if (fd < 0)
    {
        printf("file %s open failed!\r\n", argv[1]);
        return -1;
    }
    unsigned char databuf[1];
    databuf[0] = atoi(argv[2]);
    int ret_value = ret_value = write(fd, databuf, sizeof(databuf));
    if (ret_value <= 0)
    {
        printf("LED Control Failed!\r\n");
        close(fd);
        return -1; 
    }
    close(fd);
    return 0;
}
```
## 运行测试
### 加载并测试驱动
1. 挂载驱动
    ```bash
    depmod
    modprobe newchrled
    ls /dev/newchrled -l
    ```
2. 测试驱动
    ```bash
    ./ledApp /dev/newchrled 1
    ./ledApp /dev/newchrled 0
    ```