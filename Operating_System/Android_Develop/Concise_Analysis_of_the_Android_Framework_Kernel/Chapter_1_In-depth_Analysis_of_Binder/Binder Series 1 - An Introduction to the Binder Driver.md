# 第二节 Binder Driver 初探
[Binder Driver](https://gityuan.com/2015/11/01/binder-driver/)
## Binder 驱动概述
### 概述
Binder 驱动是 Android 专用的，但底层的驱动架构与Linux驱动一样。binder 驱动在以 misc 设备进行注册，作为虚拟字符设备，没有直接操作硬件，只是对设备内存的处理。主要是驱动设备的初始化`binder_init`，打开`binder_open`，映射`binder_mmap`，数据操作`binder_ioctl`。

### 系统调用
用户态的程序调用 Kernel 层驱动是需要陷入内核态，进行系统调用
## Binder 核心方法
具体可以查阅源码
[kernel/drivers/staging/android/binder.c](Chapter_1_In-depth_Analysis_of_Binder\binder.c)
[kernel/drivers/staging/android/binder_alloc.c](Chapter_1_In-depth_Analysis_of_Binder\binder_alloc.c)
1. `binder_init` 注册 misc 设备
    使用 `debugfs_create_dir` 需要在 `/kernel/arch/arm64/configs/` 中打开 `CONFIG_DEBUG_FS`
    - `misc_register`
        注册 misc 设备，`miscdevice` 结构体 和 `file_operations` 结构体
2. `binder_open` 打开 binder 驱动设备
    创建 `binder_proc` 对象，并把当前进程等信息保存到 `binder_proc` 对象，该对象管理IPC所需的各种信息并拥有其他结构体的根结构体；再把 `binder_proc` 对象保存到文件指针 `filp` ，以及吧 `binder_proc` 加到全局链表 `binder_procs` 中
3. `binder_mmap` 建立一块 Binder 通信缓冲区映射，用户虚拟内存空间
    该函数主要功能首先在内核虚拟地址空间，申请一块与用户虚拟内存相同大小的内存，然后再申请一个page大小的物理内存，再将同一块物理内存分别映射到内核虚拟地址空间和用户虚拟内存空间，从而实现了用户空间的buffer和内核空间的buffer同步操作的功能，`binder_mmap`通过加锁，保证一次只有一个进程分配内存，保证多进程间的并发访问
    - `user_buffer_offset`
        是虚拟进程地址与虚拟内存地址的差值，进程地址减去虚拟内存地址，就是用户空间和内核空间之间的偏移
4. `binder_update_page_range` 在 Binder 的虚拟映射区内，分配/释放实际页并更新页表映射
    这个函数先分配一个 page 的物理内存，然后将物理内存分别映射到虚拟进程空间和虚拟内存空间
5. `binder_alloc_buf` 分配 `binder_buffer` 结构体，只有在 `binder_transaction` 过程中才需要分配 buffer
6. `binder_ioctl` 执行相应的ioctl操作，该过程需要持有 binder_main_lock 同步锁
7. `binder_get_thread` 从 `binder_proc` 中查找当前 `binder_thread`, 如果当前线程已经加入到 proc 的线程队列则直接返回，如果不存在则创建 `binder_thread` ，并将当前线程添加到当前的 proc 
8. `binder_ioctl_write_read` 把用户空间待发送的 Binder 命令读入内核，再把内核待返回的 Binder 结果写回用户空间
    - `binder_write_read` 是整个 Binder IPC 过程，最为核心的数据结构之一
        ![alt text](Chapter_1_In-depth_Analysis_of_Binder\BWR_Core_Data_Charts.png)
## 数据结构
1. `binder_proc` 
    表示打开一个 `/dev/binder` 的进程在 Binder 驱动中的上下文，管理该进程的 Binder 资源。包含进程对应的线程列表，节点列表、引用列表、待处理工作队列、内存映射区等信息
2. `binder_thread` 
    表示某个进程中的一个 Binder 工作线程，代表当前参与 Binder通信的线程上下文，主要管理线程的事务栈、待处理事务、返回结果以及线程状态
3. `binder_node`
    表示一个 Binder 实体对象，是服务端本地 Binder 对象在内核中的对应表示，用于描述一个可被远程进程访问的 Binder 服务实体
4. `binder_ref` 
    表示某个进程对一个 `binder_node` 的引用，是用户态 handle 到内核实体 `binder_node` 的映射；客户端通常不是直接操作访问 `binder_node` ，而是通过 `binder_ref` 来进行访问
5. `binder_ref_death` 
    表示 Binder 引用的死亡通知消息，用于目标 Binder 实体死亡时间向客户端发送死亡回调，其中 `cookie` 是用户空间注册死亡通知时附带的标示，用于回调时定位对应的代理对象
6. `binder_write_read` 
    用户空间程序和 Binder 驱动程序交互基本都是通过 `BINDER_WRITE_READ` 命令，用于描述写入内核的缓冲区和从内核读取结果的缓冲区，是 Binder 读写交换的基础结构
7. `binder_transaction_data` 
    表示一次 Binder 事务的核心描述信息，用来在用户空间和内核空间之间描述一次 IPC 调用，其中包含目标对象、事务码、标志、数据区地址、偏移区地址大小等信息
8. `flat_binder_object` 
    代表 Binder 对象在两个进程间传递的扁平结构，用于在 `Parcel` 中传递 Binder 实体、Binder 引用或文件描述符等特殊对象
9. `binder_buffer` 
    每次 binder 传输数据，都会从内存缓存区中分配一个 `binder_buffer` 来存储传输数据
10. `binder_transaction` 
    表示内核中的一次 Binder 事务对象，用于描述一次完整的请求或回复过程，他会关联发送线程、目标线程、目标节点、事务数据以及事务栈关系
11. `binder_work`
    表示 Binder 驱动中一个待处理工作项、是 Binder 任务调度的基本单位
12. `binder_state` 
    通常描述用户空间 Binder 设备状态