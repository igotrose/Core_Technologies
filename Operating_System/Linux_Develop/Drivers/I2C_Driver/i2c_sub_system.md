# Linux I2C 驱动
`I2C`是一种很常见的同步、串行、低速、近距离通信接口，用于连接各种IC、传感器等器件，它们都会提供`I2C`接口与SoC主控相连，比如陀螺仪、加速度计、触摸屏等，其最大优势在于可以在总线上扩展多个外围设备的支持，以下内容是为了熟悉Linux I2C 驱动的一些基本概念和操作方法。
## I2C 总线框架
### I2C 子系统
内核开发者为了让驱动工程师在内核中方便的添加自己的I2C设备驱动程序，方便大家共同开发，Linux 2.6.23 版本引入了`I2C`子系统，它是一套完整的I2C驱动框架，它包含了I2C总线的驱动、I2C设备驱动、I2C主机驱动、I2C核心驱动等模块，如下图所示：
1. **I2C-Core**
    I2C-Core模块是I2C驱动的核心模块，提供总线驱动和设备的注册、注销方法、通信方法与具体的硬件无关
2. **I2C-adapter**
    I2C-adapter模块是I2C设备驱动的适配器模块，它负责将I2C设备驱动与I2C总线驱动进行绑定，并向上提供设备驱动接口，向下提供I2C总线操作接口。
3. **I2C-client-driver**
    包括了设备的注册和驱动管理
### I2C 总线驱动
I2C总线驱动的重点是I2C适配器，这里会用到两个重要的数据结构：`i2c_adapter`和`i2c_algorithm`
1. `i2c_adapter`是I2C适配器的数据结构，它包含了I2C总线的一些基本信息，比如总线ID、总线速度、总线地址等；
    ```C
    /*
    * i2c_adapter is the structure used to identify a physical i2c bus along
    * with the access algorithms necessary to access it.
    */
    struct i2c_adapter {
        struct module *owner;
        unsigned int class;		  /* classes to allow probing for */
        const struct i2c_algorithm *algo; /* the algorithm to access the bus */
        void *algo_data;

        /* data fields that are valid for all devices	*/
        const struct i2c_lock_operations *lock_ops;
        struct rt_mutex bus_lock;
        struct rt_mutex mux_lock;

        int timeout;			/* in jiffies */
        int retries;
        struct device dev;		/* the adapter device */
        unsigned long locked_flags;	/* owned by the I2C core */
    #define I2C_ALF_IS_SUSPENDED		0
    #define I2C_ALF_SUSPEND_REPORTED	1

        int nr;
        char name[48];
        struct completion dev_released;

        struct mutex userspace_clients_lock;
        struct list_head userspace_clients;

        struct i2c_bus_recovery_info *bus_recovery_info;
        const struct i2c_adapter_quirks *quirks;

        struct irq_domain *host_notify_domain;
    };
    ```
    - `owner`：模块所有者
    - `class`：允许探测的类别
    - `algo`：访问总线的算法
    - `algo_data`：算法数据
    - `lock_ops`：锁操作
    - `bus_lock`：总线锁
    - `mux_lock`：多路复用锁
    - `timeout`：超时时间
    - `retries`：重试次数
    - `dev`：适配器设备
    - `locked_flags`：由I2C核心拥有的标志
    - `nr`：总线号
    - `name`：总线名称
    - `dev_released`：设备释放完成
    - `userspace_clients_lock`：用户空间客户端锁
    - `userspace_clients`：用户空间客户端列表
    - `bus_recovery_info`：总线恢复信息
    - `quirks`：适配器的缺陷
    - `host_notify_domain`：主机通知域
2. `i2c_algorithm`是I2C设备驱动的算法，它包含了设备的读写、中断处理等方法
    ```c
    /**
     * struct i2c_algorithm - represent I2C transfer method
     * @master_xfer: Issue a set of i2c transactions to the given I2C adapter
     *   defined by the msgs array, with num messages available to transfer via
     *   the adapter specified by adap.
     * @master_xfer_atomic: same as @master_xfer. Yet, only using atomic context
     *   so e.g. PMICs can be accessed very late before shutdown. Optional.
     * @smbus_xfer: Issue smbus transactions to the given I2C adapter. If this
     *   is not present, then the bus layer will try and convert the SMBus calls
     *   into I2C transfers instead.
     * @smbus_xfer_atomic: same as @smbus_xfer. Yet, only using atomic context
     *   so e.g. PMICs can be accessed very late before shutdown. Optional.
     * @functionality: Return the flags that this algorithm/adapter pair supports
     *   from the ``I2C_FUNC_*`` flags.
     * @reg_slave: Register given client to I2C slave mode of this adapter
     * @unreg_slave: Unregister given client from I2C slave mode of this adapter
     *
     * The following structs are for those who like to implement new bus drivers:
     * i2c_algorithm is the interface to a class of hardware solutions which can
     * be addressed using the same bus algorithms - i.e. bit-banging or the PCF8584
     * to name two of the most common.
     *
     * The return codes from the ``master_xfer{_atomic}`` fields should indicate the
     * type of error code that occurred during the transfer, as documented in the
     * Kernel Documentation file Documentation/i2c/fault-codes.rst.
     */
    struct i2c_algorithm {
        /*
        * If an adapter algorithm can't do I2C-level access, set master_xfer
        * to NULL. If an adapter algorithm can do SMBus access, set
        * smbus_xfer. If set to NULL, the SMBus protocol is simulated
        * using common I2C messages.
        *
        * master_xfer should return the number of messages successfully
        * processed, or a negative value on error
        */
        int (*master_xfer)(struct i2c_adapter *adap, struct i2c_msg *msgs,
                int num);
        int (*master_xfer_atomic)(struct i2c_adapter *adap,
                    struct i2c_msg *msgs, int num);
        int (*smbus_xfer)(struct i2c_adapter *adap, u16 addr,
                unsigned short flags, char read_write,
                u8 command, int size, union i2c_smbus_data *data);
        int (*smbus_xfer_atomic)(struct i2c_adapter *adap, u16 addr,
                    unsigned short flags, char read_write,
                    u8 command, int size, union i2c_smbus_data *data);

        /* To determine what the adapter supports */
        u32 (*functionality)(struct i2c_adapter *adap);

    #if IS_ENABLED(CONFIG_I2C_SLAVE)
        int (*reg_slave)(struct i2c_client *client);
        int (*unreg_slave)(struct i2c_client *client);
    #endif
    };
    ```
    - `master_xfer`：I2C事务传输方法
    - `master_xfer_atomic`：原子I2C事务传输方法
    - `smbus_xfer`：SMBus事务传输方法
    - `smbus_xfer_atomic`：原子SMBus事务传输方法
    - `functionality`：返回适配器支持的功能标志
    - `reg_slave`：注册客户端到I2C从机模式
    - `unreg_slave`：注销客户端从I2C从机模式

所以I2C总线驱动的主要工作就是实现`i2c_adapter`和`i2c_algorithm`的数据结构，设置`i2c_algorithm`中的`master_xfer`函数，完成以后通过`i2c_add_adapter`或者`i2c_add_numbered_adapter`这两个函数向I2C子系统注册设置好的`i2c_adapter`，原型如下
```c
int i2c_add_adapter(struct i2c_adapter *adapter);
int i2c_add_numbered_adapter(struct i2c_adapter *adapter);
```
- `adapter`：I2C适配器
返回值：成功返回0，失败返回负值

这两个函数的区别在于前者会动态分配一个总线编号，后者则需要指定一个总线编号。   
如果要删除I2C适配器，则可以通过调用`i2c_del_adapter`函数，原型如下
```c
void i2c_del_adapter(struct i2c_adapter *adapter);
```
- `adapter`：I2C适配器

一般Soc的I2C总线都是由半导体厂商提供的，所以Linux I2C驱动程序一般都是由厂商提供的，厂商提供的驱动程序一般都比较成熟，所以Linux I2C驱动程序的编写难度一般不高。我们主要专注驱动就好了
### I2C总线设备
I2C设备驱动重点关注两个数据结构：`i2c_client`和`i2c_driver`
1. `i2c_client`是I2C设备的数据结构，它包含了设备的一些基本信息，比如设备地址、设备名称等；一个I2C设备可以有多个`i2c_client`，比如一个传感器设备可以有多个传感器通道，每个通道都是一个`i2c_client`；
    ```c
    /**
     * struct i2c_client - represent an I2C slave device
     * @flags: see I2C_CLIENT_* for possible flags
     * @addr: Address used on the I2C bus connected to the parent adapter.
     * @name: Indicates the type of the device, usually a chip name that's
     *	generic enough to hide second-sourcing and compatible revisions.
    * @adapter: manages the bus segment hosting this I2C device
    * @dev: Driver model device node for the slave.
    * @init_irq: IRQ that was set at initialization
    * @irq: indicates the IRQ generated by this device (if any)
    * @detected: member of an i2c_driver.clients list or i2c-core's
    *	userspace_devices list
    * @slave_cb: Callback when I2C slave mode of an adapter is used. The adapter
    *	calls it to pass on slave events to the slave driver.
    *
    * An i2c_client identifies a single device (i.e. chip) connected to an
    * i2c bus. The behaviour exposed to Linux is defined by the driver
    * managing the device.
    */
    struct i2c_client {
        unsigned short flags;		/* div., see below		*/
    #define I2C_CLIENT_PEC		0x04	/* Use Packet Error Checking */
    #define I2C_CLIENT_TEN		0x10	/* we have a ten bit chip address */
                        /* Must equal I2C_M_TEN below */
    #define I2C_CLIENT_SLAVE	0x20	/* we are the slave */
    #define I2C_CLIENT_HOST_NOTIFY	0x40	/* We want to use I2C host notify */
    #define I2C_CLIENT_WAKE		0x80	/* for board_info; true iff can wake */
    #define I2C_CLIENT_SCCB		0x9000	/* Use Omnivision SCCB protocol */
                        /* Must match I2C_M_STOP|IGNORE_NAK */

        unsigned short addr;		/* chip address - NOTE: 7bit	*/
                        /* addresses are stored in the	*/
                        /* _LOWER_ 7 bits		*/
        char name[I2C_NAME_SIZE];
        struct i2c_adapter *adapter;	/* the adapter we sit on	*/
        struct device dev;		/* the device structure		*/
        int init_irq;			/* irq set at initialization	*/
        int irq;			/* irq issued by device		*/
        struct list_head detected;
    #if IS_ENABLED(CONFIG_I2C_SLAVE)
        i2c_slave_cb_t slave_cb;	/* callback for slave mode	*/
    #endif
    };
    ```
    - `flags`：设备标志
    - `addr`：设备地址
    - `name`：设备名称
    - `adapter`：设备所在的总线
    - `dev`：设备模型节点
    - `init_irq`：初始化时使用的中断
    - `irq`：设备使用的中断
    - `detected`：设备是否被探测到
    - `slave_cb`：I2C从机模式回调函数
2. `i2c_driver`是I2C设备驱动的结构，它包含了设备的初始化、反初始化、操作等方法
    ```c
    /**
     * struct i2c_driver - represent an I2C device driver
    * @class: What kind of i2c device we instantiate (for detect)
    * @probe: Callback for device binding - soon to be deprecated
    * @probe_new: New callback for device binding
    * @remove: Callback for device unbinding
    * @shutdown: Callback for device shutdown
    * @alert: Alert callback, for example for the SMBus alert protocol
    * @command: Callback for bus-wide signaling (optional)
    * @driver: Device driver model driver
    * @id_table: List of I2C devices supported by this driver
    * @detect: Callback for device detection
    * @address_list: The I2C addresses to probe (for detect)
    * @clients: List of detected clients we created (for i2c-core use only)
    *
    * The driver.owner field should be set to the module owner of this driver.
    * The driver.name field should be set to the name of this driver.
    *
    * For automatic device detection, both @detect and @address_list must
    * be defined. @class should also be set, otherwise only devices forced
    * with module parameters will be created. The detect function must
    * fill at least the name field of the i2c_board_info structure it is
    * handed upon successful detection, and possibly also the flags field.
    *
    * If @detect is missing, the driver will still work fine for enumerated
    * devices. Detected devices simply won't be supported. This is expected
    * for the many I2C/SMBus devices which can't be detected reliably, and
    * the ones which can always be enumerated in practice.
    *
    * The i2c_client structure which is handed to the @detect callback is
    * not a real i2c_client. It is initialized just enough so that you can
    * call i2c_smbus_read_byte_data and friends on it. Don't do anything
    * else with it. In particular, calling dev_dbg and friends on it is
    * not allowed.
    */
    struct i2c_driver {
        unsigned int class;

        /* Standard driver model interfaces */
        int (*probe)(struct i2c_client *client, const struct i2c_device_id *id);
        int (*remove)(struct i2c_client *client);

        /* New driver model interface to aid the seamless removal of the
        * current probe()'s, more commonly unused than used second parameter.
        */
        int (*probe_new)(struct i2c_client *client);

        /* driver model interfaces that don't relate to enumeration  */
        void (*shutdown)(struct i2c_client *client);

        /* Alert callback, for example for the SMBus alert protocol.
        * The format and meaning of the data value depends on the protocol.
        * For the SMBus alert protocol, there is a single bit of data passed
        * as the alert response's low bit ("event flag").
        * For the SMBus Host Notify protocol, the data corresponds to the
        * 16-bit payload data reported by the slave device acting as master.
        */
        void (*alert)(struct i2c_client *client, enum i2c_alert_protocol protocol,
                unsigned int data);

        /* a ioctl like command that can be used to perform specific functions
        * with the device.
        */
        int (*command)(struct i2c_client *client, unsigned int cmd, void *arg);

        struct device_driver driver;
        const struct i2c_device_id *id_table;

        /* Device detection callback for automatic device creation */
        int (*detect)(struct i2c_client *client, struct i2c_board_info *info);
        const unsigned short *address_list;
        struct list_head clients;
    };
    ```
    - `class`：设备类别
    - `probe`：设备绑定回调函数
    - `remove`：设备解绑回调函数
    - `probe_new`：新设备绑定回调函数
    - `shutdown`：设备关闭回调函数
    - `alert`：设备告警回调函数
    - `command`：设备命令回调函数
    - `driver`：设备驱动模型结构
    - `id_table`：设备ID表
    - `detect`：设备探测回调函数
    - `address_list`：探测的设备地址列表 
    - `clients`：探测到的设备列表

`i2c_client`和`i2c_driver`的关系类似于`platform`总线下的`platform_device`和`platform_driver`，对于设备驱动编写人员来说，重点工作就是构建`i2c_driver`，构建完成以后需要向I2C子系统注册这个`i2c_driver`，通过`i2c_register_driver`函数注册，原型如下
```c
int i2c_register_driver(struct module *owner, struct i2c_driver *driver);
```
- `owner`：驱动模块所有者
- `driver`：I2C设备驱动
返回值：成功返回0，失败返回负值

另外也可以使用`i2c_add_driver`函数注册，原型如下
```c
#define i2c_add_driver(driver) \
	i2c_register_driver(THIS_MODULE, driver)
```
- `driver`：I2C设备驱动
返回值：成功返回0，失败返回负值

注销I2C设备驱动可以通过调用`i2c_del_driver`函数，原型如下
```c
static int xxx_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    return 0;
}

static int xxx_remove(struct i2c_client *client)
{
    return 0; 
}

void i2c_del_driver(struct i2c_driver *driver);
```
- `driver`：I2C设备驱动

`i2c_driver`的注册示例代码如下
```c

static const struct i2c_device_id xxx_id_table[] = {
    {"xxx", 0},
    {}
};

static const struct of_device_id xxx_of_match[] = {
    {.compatible = "xxx,xxx" },
    {}
}; 

static struct i2c_driver xxx_driver = {
    .probe = xxx_probe,
    .remove = xxx_remove,
    .id_table = xxx_id_table,
    .driver = {
        .name = "xxx",
        .owner = THIS_MODULE,
        .of_match_table = xxx_of_match,
        },
}

static int __init xxx_init(void)
{
    int ret;
    ret = i2c_add_driver(&xxx_driver);
    return ret;
}

static void __exit xxx_exit(void)
{
    i2c_del_driver(&xxx_driver);
}

module_init(xxx_init);
module_exit(xxx_exit);
```

### I2C设备和驱动的匹配过程
I2C设备和驱动的匹配过程是由I2C子系统核心层来完成的，I2C总线的数据结构体为`i2c_bus_type`，其定义如下
```c
struct bus_type i2c_bus_type = {
	.name		= "i2c",
	.match		= i2c_device_match,
	.probe		= i2c_device_probe,
	.remove		= i2c_device_remove,
	.shutdown	= i2c_device_shutdown,
};
```
- `name`：总线名称
- `match`：匹配回调函数
- `probe`：探测回调函数
- `remove`：移除回调函数
- `shutdown`：关闭回调函数

当I2C子系统收到I2C总线的探测请求时，会调用`i2c_bus_type.match`函数，这个函数会遍历所有注册的`i2c_driver`，调用每个`i2c_driver`的`probe`函数，如果`probe`函数返回0，则说明匹配成功，否则继续遍历下一个`i2c_driver`，直到所有`i2c_driver`都没有匹配成功，则说明没有匹配成功。

如果匹配成功，则调用`i2c_bus_type.probe`函数，这个函数会调用`i2c_driver`的`probe`函数，如果`probe`函数返回0，则说明探测成功，否则继续遍历下一个`i2c_driver`，直到所有`i2c_driver`都没有探测成功，则说明探测失败。

如果探测成功，则调用`i2c_bus_type.remove`函数，这个函数会调用`i2c_driver`的`remove`函数，如果`remove`函数返回0，则说明移除成功，否则继续遍历下一个`i2c_driver`，直到所有`i2c_driver`都没有移除成功，则说明移除失败。

如果移除成功，则调用`i2c_bus_type.shutdown`函数，这个函数会调用`i2c_driver`的`shutdown`函数，如果`shutdown`函数返回0，则说明关闭成功，否则继续遍历下一个`i2c_driver`，直到所有`i2c_driver`都没有关闭成功，则说明关闭失败。
## I2C设备驱动编写流程
### 使用设备树的时候
使用设备树的时候I2C设备信息通过创建相应的节点就行，设备树的具体配置可以查看Soc的参考手册和外设的参考手册。
```dts
&i2c0 {
    status = "okay";
    pinctrl-names = "default";
    pinctrl-0 = <&i2c0_pins>;
    xxx_device {
        compatible = "xxx,xxx";
        reg = <0x0a>;
    }；
    xxx_device1 {
        compatible = "xxx,xxx";
        reg = <0x0b>;
    }；
};
```
- `i2c0`：I2C总线节点
- `status`：节点状态
- `pinctrl-names`：引脚控制名称
- `pinctrl-0`：引脚控制节点
- `xxx_device`：设备节点
- `compatible`：设备兼容字符串
- `reg`：设备地址

### I2C设备数据收发处理流程
I2C设备数据收发处理流程如下：
1. 打开I2C设备
2. 设置I2C设备地址
3. 发送或接收数据
4. 关闭I2C设备

使用`i2c_transfer`函数可以实现I2C设备数据收发处理流程，其原型如下
```c
int i2c_transfer(struct i2c_adapter *adapter, struct i2c_msg *msgs, int num);
```
- `adapter`：I2C总线适配器
- `msgs`：I2C消息数组
- `num`：I2C消息数组长度

`i2c_msg`结构体定义如下
```c
struct i2c_msg {
	__u16 addr;		/* slave address			*/
	__u16 flags;		/* see i2c_msg_flags			*/
	__u16 len;		/* msg length				*/
	__u8 *buf;		/* pointer to msg data			*/
};
```
- `addr`：设备地址
- `flags`：消息标志
- `len`：消息长度
- `buf`：消息数据指针

使用`i2c_transfer`函数发送数据之前一定要先构建好`i2c_msg`数组，数组中至少要包含一个`i2c_msg`结构体，`addr`字段指定设备地址，`flags`字段指定消息标志，`len`字段指定消息长度，`buf`字段指定消息数据指针。还有两个API函数用于发送或接收数据，分别是`i2c_master_send`和`i2c_master_recv`，其原型如下
```c
int i2c_master_send(struct i2c_client *client, const char *buf, int count);
int i2c_master_recv(struct i2c_client *client, char *buf, int count);
```
- `client`：I2C客户端
- `buf`：消息数据指针
- `count`：消息长度

