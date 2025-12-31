# 异步通知实验
## 异步通知
### 异步通知简介
异步通知是一种在设备驱动程序中实现的通知机制，它允许驱动程序向用户空间通知设备状态的变化。异步通知机制可以减少用户空间对设备的轮询，从而提高设备的响应速度。异步通知的核心是信号，在`arch/xtensa/include/asm/signal.h`中定义。信号是一种软件中断，它可以被发送到任意进程，并在目标进程中执行。下面是内核中的信号定义：

```c
#define SIGHUP         1       /* Hangup (POSIX) */
#define SIGINT         2       /* Interrupt (ANSI) */
#define SIGQUIT        3       /* Quit (POSIX) */
#define SIGILL         4       /* Illegal instruction (ANSI) */
#define SIGTRAP        5       /* Trace trap (POSIX) */
#define SIGABRT        6       /* Abort (ANSI) */
#define SIGBUS         7       /* BUS error (4.2BSD) */
#define SIGFPE         8       /* Floating-point exception (ANSI) */
#define SIGKILL        9       /* Kill (POSIX) */
#define SIGUSR1        10      /* User-defined signal 1 (POSIX) */
#define SIGSEGV        11      /* Segmentation violation (ANSI) */
#define SIGUSR2        12      /* User-defined signal 2 (POSIX) */
#define SIGPIPE        13      /* Broken pipe (POSIX) */
#define SIGALRM        14      /* Alarm clock (POSIX) */
#define SIGTERM        15      /* Termination (ANSI) */
#define SIGSTKFLT      16      /* Stack fault */
#define SIGCHLD        17      /* Child status has changed (POSIX) */
#define SIGCONT        18      /* Continue (POSIX) */
#define SIGSTOP        19      /* Stop, unblockable (POSIX) */
#define SIGTSTP        20      /* Keyboard stop (POSIX) */
#define SIGTTIN        21      /* Background read from tty (POSIX) */
#define SIGTTOU        22      /* Background write to tty (POSIX) */
#define SIGURG         23      /* Urgent condition on socket (4.2BSD) */
#define SIGXCPU        24      /* CPU time limit exceeded (4.2BSD) */
#define SIGXFSZ        25      /* File size limit exceeded (4.2BSD) */
#define SIGVTALRM      26      /* Virtual alarm clock (4.2BSD) */
#define SIGPROF        27      /* Profiling alarm clock (4.2BSD) */
#define SIGWINCH       28      /* Window size change (4.3BSD, Sun) */
#define SIGIO          29      /* I/O now possible (4.2BSD) */
#define SIGPWR         30      /* Power failure restart (System V) */
#define SIGSYS         31      /* Bad system call */
```
这些信号中，除了`SIGKILL(9)`和`SIGSTOP(19)`，其他信号都可以被用户空间进程捕获。当设备状态发生变化时，驱动程序可以发送信号给相应的进程，通知用户空间进程设备状态的变化。
### 驱动中的信号处理
1. `fasync_struct`结构体
    ```c
    struct fasync_struct {
        rwlock_t		fa_lock;
        int			magic;
        int			fa_fd;
        struct fasync_struct	*fa_next; /* singly linked list */
        struct file		*fa_file;
        struct rcu_head		fa_rcu;
    };
    ```
    一般将该结构体指针变量定义到设备结构体中，例如：
    ```c
    struct key_dev
    {
        dev_t devid;
        struct cdev cdev;
        struct class* class;
        struct device* device;
        struct device_node* nd;
        ...
        struct fasync_struct *async_queue;
    };
    ```
    其中`async_queue`指向`fasync_struct`结构体的指针。
2. `fasync`函数
    如果要使用异步通知，需要在驱动设备中实现`file_operations`结构体中的`fasync`函数。该函数的作用是将信号发送到相应的进程。函数格式如下：
    ```c
    int fasync(int fd, struct file *file, int on)
    ```
    - `fd`：文件描述符
    - `file`：指向`file`结构体的指针
    - `on`：打开或关闭异步通知。`on`为1表示打开异步通知，为0表示关闭异步通知。
    函数返回0表示成功，其他值表示失败。

    `fasync`函数里面一般通过调用`fasync_helper`函数来实现异步通知。原型如下：
    ```c
    int fasync_helper(int fd, struct file *file, int on, struct fasync_struct **list)
    ```
    - `fd`：文件描述符
    - `file`：指向`file`结构体的指针
    - `on`：打开或关闭异步通知。`on`为1表示打开异步通知，为0表示关闭异步通知。
    - `list`：指向`fasync_struct`结构体指针的指针。该指针指向`file`结构体中的`async_queue`指针。
    函数返回0表示成功，其他值表示失败。

    当应用程序通过`fcntl(fd, F_SETFL, O_ASYNC)`改变文件描述符的异步通知状态时，内核会调用`fasync_helper`函数，并将`on`参数设置为1或0。

    驱动程序的fasync函数参考示例如下：
    ```c
    struct xxx_dev{
        ...
        struct fasync_struct *async_queue;
    };

    static int xxx_fasync(int fd, struct file *file, int on)
    {
        return fasync_helper(fd, file, on, &dev->async_queue);
    }

    static const struct file_operations xxx_fops = {
        ... 
        .fasync = xxx_fasync,
        .release = xxx_release,
        ...
    };

    static int xxx_release(struct inode *inode, struct file *file)
    {
        struct xxx_dev *dev = container_of(file->private_data, struct xxx_dev, cdev);
        return xxx_fasync(-1, file, 0);     /* close async notification */
    }
    ```
    在`xxx_fasync`函数中，通过调用`fasync_helper`函数将信号发送到相应的进程。在`xxx_release`函数中，调用`xxx_fasync`函数关闭异步通知。可实际上是通过`kill_fasync`函数来完成释放发送信号的工作。函数原型如下：
    ```c
    void kill_fasync(struct fasync_struct *fa, int sig, int band)
    ```
    - `fa`：指向`fasync_struct`结构体的指针
    - `sig`：信号值
    - `band`：信号掩码
    函数返回0表示成功，其他值表示失败。
### 应用程序对异步通知的处理
1. 注册信号处理函数
    应用程序可以通过`signal`函数注册信号处理函数，并将信号值设置为`SIGIO`。信号处理函数的原型如下：
    ```c
    void signal_handler(int sig)
    ```
    该函数的作用是处理异步通知信号。
2. 打开关闭异步通知
    应用程序可以通过`fcntl`函数打开关闭异步通知。函数原型如下：
    ```c
    int fcntl(int fd, int cmd, long arg)          
    ```
    - `fd`：文件描述符
    - `cmd`：fcntl命令
    - `arg`：命令参数
    其中`cmd`参数可以是`F_SETFL`或`F_GETFL`，`arg`参数可以是`O_ASYNC`或`0`。

    应用程序可以通过`fcntl(fd, F_SETFL, O_ASYNC)`打开异步通知，通过`fcntl(fd, F_SETFL, 0)`关闭异步通知。
3. 等待异步通知
    应用程序可以通过`select`函数等待异步通知。函数原型如下：
    ```c
    int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
    ```
    - `nfds`：文件描述符的最大值+1
    - `readfds`：等待读事件的文件描述符集合
    - `writefds`：等待写事件的文件描述符集合
    - `exceptfds`：等待异常事件的文件描述符集合
    - `timeout`：超时时间
## 实验
### 修改设备树文件 
### 驱动程序
```c
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>   
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/irq.h>
#include <linux/of_irq.h>
#include <linux/timer.h>
#include <linux/fcntl.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define KEY_CNT     1
#define KEY_NAME    "key"

enum key_status
{
    KEY_PRESS = 0,
    KEY_RELEASE,
    KEY_KEEP,
};

struct key_dev
{
    dev_t devid;
    struct cdev cdev;
    struct class* class;
    struct device* device;
    struct device_node* nd;
    int key_gpio;
    struct timer_list timer;
    int irq_num;

    atomic_t status;
    wait_queue_head_t r_queue;
    struct fasync_struct* async_queue;
};

static struct key_dev key;

static irqreturn_t key_interrupt(int irq, void* dev_id)
{
    /* 消抖处理 */
    mod_timer(&key.timer, jiffies + msecs_to_jiffies(50));
    return IRQ_HANDLED;
}

static int key_parse_dt(void)
{
    int ret;
    const char* str;
    key.nd = of_find_node_by_path("/key");
    if (key.nd == NULL)
    {
        printk(KERN_ERR "key: Failed to get key node.\r\n");
        return -EINVAL;
    }
    ret = of_property_read_string(key.nd, "status", &str);
    if (ret < 0)
    {
        return -EINVAL;
    }
    if (strcmp(str, "okay"))
    {
        return -EINVAL;
    }
    ret = of_property_read_string(key.nd, "compatible", &str);
    if (ret < 0)
    {
        printk(KERN_ERR "key: Failed to get compatible.\r\n");
        return -EINVAL;
    }
    if (strcmp(str, "alientek,key"))
    {
        printk(KERN_ERR "key: Incompatible compatible.\r\n");
        return -EINVAL;
    }
    key.key_gpio = of_get_named_gpio(key.nd, "key-gpios", 0);
    if (key.key_gpio < 0)
    {
        printk(KERN_ERR "key: Failed to get key0 pin.\r\n");
        return -EINVAL;
    }
    key.irq_num = gpio_to_irq(key.key_gpio);
    if (!key.irq_num)
    {
        printk(KERN_ERR "key: Failed to get key0 irq number.\r\n");
        return -EINVAL;
    }
    printk(KERN_INFO "key: key0 pin = %d, irq number = %d.\r\n", key.key_gpio, key.irq_num);

    return 0;
}

static void key_timer_function(struct timer_list* arg)
{
    static int last_val = 0;
    int current_val;

    current_val = gpio_get_value(key.key_gpio);
    if (1 == current_val && !last_val)
    {
        atomic_set(&key.status, KEY_PRESS);
        wake_up_interruptible(&key.r_queue);
        if (key.async_queue)
        {
            kill_fasync(&key.async_queue, SIGIO, POLL_IN);
        }

    }
    else if (0 == current_val && last_val)
    {
        atomic_set(&key.status, KEY_RELEASE);
        wake_up_interruptible(&key.r_queue);
        if (key.async_queue)
        {
            kill_fasync(&key.async_queue, SIGIO, POLL_IN);
        }
    }
    else
    {
        atomic_set(&key.status, KEY_KEEP);
    }
    last_val = current_val;
}
static int key_gpio_init(void)
{
    int ret;
    unsigned long irq_flags;

    ret = gpio_request(key.key_gpio, "key0");
    if (ret < 0)
    {
        printk(KERN_ERR "key: Failed to request key0 pin.\r\n");
        return ret;
    }
    gpio_direction_input(key.key_gpio);

    irq_flags = irq_get_trigger_type(key.irq_num);
    if (irq_flags == IRQF_TRIGGER_NONE)
    {
        irq_flags = IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING;
    }

    ret = request_irq(key.irq_num, key_interrupt, irq_flags, "key0_irq", &key);
    if (ret)
    {
        gpio_free(key.key_gpio);
        return ret;
    }
    return 0;
}

static int key_open(struct inode* inode, struct file* file)
{
    return 0;
}

static ssize_t key_read(struct file* file, char __user* buf, size_t count, loff_t* ppos)
{
    int ret;
    if (file->f_flags & O_NONBLOCK)
    {
        if (KEY_KEEP == atomic_read(&key.status))
        {
            return -EAGAIN;
        }
    }
    else
    {
        ret = wait_event_interruptible(key.r_queue, KEY_PRESS != atomic_read(&key.status));
        if (ret)
        {
            return -ERESTARTSYS;
        }
    }
    ret = copy_to_user(buf, &key.status, sizeof(key.status));
    atomic_set(&key.status, KEY_KEEP);
    return ret;
}

static unsigned int key_poll(struct file* file, poll_table* wait)
{
    unsigned int mask = 0;
    poll_wait(file, &key.r_queue, wait);
    if (KEY_PRESS != atomic_read(&key.status))
    {
        mask |= POLLIN | POLLRDNORM;
    }
    return mask;
}

static ssize_t key_write(struct file* file, const char __user* buf, size_t count, loff_t* ppos)
{
    return 0;
}

static int key_fasync(int fd, struct file* file, int on)
{
    return fasync_helper(fd, file, on, &key.async_queue);
}
static int key_release(struct inode* inode, struct file* file)
{
    return key_fasync(-1, file, 0);
}

static struct file_operations key_fops =
{
    .owner = THIS_MODULE,
    .open = key_open,
    .release = key_release,
    .read = key_read,
    .write = key_write,
    .poll = key_poll,
    .fasync = key_fasync,
};

static int __init mykey_init(void)
{
    int ret;

    init_waitqueue_head(&key.r_queue);

    timer_setup(&key.timer, key_timer_function, 0);

    atomic_set(&key.status, KEY_KEEP);

    ret = key_parse_dt();
    if (ret)
    {
        return ret;
    }
    ret = key_gpio_init();
    if (ret)
    {
        return ret;
    }
    ret = alloc_chrdev_region(&key.devid, 0, KEY_CNT, KEY_NAME);
    if (ret)
    {
        pr_err("key: Failed to alloc chrdev region, ret = %d.\r\n", ret);
        goto free_gpio;
    }

    key.cdev.owner = THIS_MODULE;
    cdev_init(&key.cdev, &key_fops);

    ret = cdev_add(&key.cdev, key.devid, KEY_CNT);
    if (ret)
    {
        pr_err("key: Failed to add cdev, ret = %d.\r\n", ret);
        goto del_unregister;
    }

    key.class = class_create(THIS_MODULE, KEY_NAME);
    if (IS_ERR(key.class))
    {
        ret = PTR_ERR(key.class);
        goto del_cdev;
    }

    key.device = device_create(key.class, NULL, key.devid, NULL, KEY_NAME);
    if (IS_ERR(key.device))
    {
        goto destroy_class;
    }
    return 0;
destroy_class:
    class_destroy(key.class);
del_cdev:
    cdev_del(&key.cdev);
del_unregister:
    unregister_chrdev_region(key.devid, KEY_CNT);
free_gpio:
    free_irq(key.irq_num, NULL);
    gpio_free(key.key_gpio);
    return -EIO;
}

static void __exit mykey_exit(void)
{
    cdev_del(&key.cdev);
    unregister_chrdev_region(key.devid, KEY_CNT);
    del_timer_sync(&key.timer);
    device_destroy(key.class, key.devid);
    class_destroy(key.class);
    free_irq(key.irq_num, NULL);
    gpio_free(key.key_gpio);
}

module_init(mykey_init);
module_exit(mykey_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("igotu");
MODULE_INFO(intree, "Y");


```
### 应用程序
```c
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <signal.h>

static int fd;

static void sigio_signal_func(int signum)
{ 
    unsigned int key_val = 0;
    read(fd, &key_val, sizeof(key_val));

    if (0 == key_val)
    {
        printf("Key pressed\n");
    }
    else if (1 == key_val)
    {
        printf("Key released\n");
    }
}

int main(int argc, char* argv[])
{
    int flags = 0;

    if (2 != argc)
    {
        printf("Usage\n"
            "\t ./asyncnotiApp %s\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        printf("Failed to open %s\n", argv[1]);
        return -1;
    }

    signal(SIGIO, sigio_signal_func);
    fcntl(fd, F_SETOWN, getpid());
    flags = fcntl(fd, F_GETFD);
    fcntl(fd, F_SETFL, flags | FASYNC);

    while (1)
    {
        sleep(1);
    }

    close(fd);
    return 0;
}
```
### 测试运行

 

