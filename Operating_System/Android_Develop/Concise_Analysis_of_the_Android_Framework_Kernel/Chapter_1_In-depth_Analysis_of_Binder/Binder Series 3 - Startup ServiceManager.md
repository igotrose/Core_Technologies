# 启动 ServiceManager

## 概述
`ServiceManager` 是 Android 中 Binder 的上下文管理者，它的核心职责不是直接处理业务逻辑，而是负责管理系统服务的注册、查询、列举以及相关通知。

对于 Binder 通信来说，`ServiceManager` 是其他进程通常首先接触到的“已知服务”。当某个进程成为 Binder 的上下文管理者之后，其他进程就可以通过固定入口向它查询目标服务。

本文主要围绕两个问题展开：

1. `ServiceManager` 是如何启动并成为 Binder 上下文管理者的
2. `ServiceManager.cpp` 在启动之后主要负责管理哪些内容

## 1. 启动流程
`ServiceManager` 是由 `init` 进程通过解析 `init.rc` 启动的，其可执行文件通常是：

```bash
/system/bin/servicemanager
```

它的启动过程可以概括为 3 步：

1. 打开 Binder 驱动
2. 将自己注册为 Binder 上下文管理者
3. 进入 Binder 循环，等待请求到来

粗略流程如下：

```text
init
 -> 启动 servicemanager 进程
 -> 打开 /dev/binder
 -> mmap Binder 缓冲区
 -> ioctl 设置 context manager
 -> 进入 Binder 线程循环
 -> 等待 addService/getService/checkService 等请求
```

## 2. 为什么 ServiceManager 很特殊
`ServiceManager` 不是一个普通的系统服务，它是 Binder 服务名的管理者。

它的主要职责包括：

- 接收服务注册
- 接收服务查询
- 返回目标服务对应的 Binder 句柄
- 管理服务注册通知和客户端回调
- 当 Binder 死亡时移除失效服务

可以简单理解为：

- 普通服务：提供某种具体功能
- `ServiceManager`：管理“服务在哪里”

## 3. 启动过程和 Binder 驱动的关系
`ServiceManager` 启动过程中的关键点在于，它会调用 Binder 驱动，将自己标记为上下文管理者。

这个动作在驱动侧对应 `binder_ioctl_set_ctx_mgr`。

它的含义是：

- 告诉 Binder 驱动，当前进程就是这个 Binder 域中的服务管理者
- 后续其他进程可以通过固定入口找到它
- 然后通过它完成服务注册与服务查询

所以 `ServiceManager` 启动的本质就是：

```text
成为 Binder 驱动认可的唯一管理者角色
```

## 4. 如何阅读 ServiceManager.cpp
阅读 `ServiceManager.cpp` 时，不要把它只当成一个“启动文件”。它的启动部分其实并不是这个文件的重点，更重要的是它在启动之后如何管理服务。

这个文件大致可以分成几个功能块：

1. 权限检查与声明检查
2. 服务查询
3. 服务注册
4. 通知回调管理
5. Binder 死亡处理
6. 客户端回调与懒加载服务支持

## 5. ServiceManager.cpp 中维护的核心数据
从代码结构上看，`ServiceManager` 主要维护了几类映射关系：

- `mNameToService`
  将服务名映射到具体服务对象
- `mNameToRegistrationCallback`
  将服务名映射到注册通知回调列表
- `mNameToClientCallback`
  将服务名映射到客户端状态回调列表

这说明 `ServiceManager` 本质上是在围绕“服务名 -> 服务对象”这张表进行管理。

## 6. 核心函数解析

### 6.1 `getService`
```cpp
Status ServiceManager::getService(const std::string& name, sp<IBinder>* outBinder)
```

作用：

- 根据服务名查询服务
- 如果服务不存在，则尝试启动对应的懒加载服务

它内部调用的是：

```cpp
tryGetService(name, true)
```

其中这个 `true` 表示：

- 如果服务没找到，就尝试调用 `tryStartService(name)`

所以 `getService` 更像是“帮我拿到这个服务，如果还没起来就尽量把它拉起来”。

### 6.2 `checkService`
```cpp
Status ServiceManager::checkService(const std::string& name, sp<IBinder>* outBinder)
```

作用：

- 查询某个服务当前是否已经存在
- 不主动启动服务

它内部调用的是：

```cpp
tryGetService(name, false)
```

所以 `checkService` 更像是一次非阻塞的存在性检查。

### 6.3 `tryGetService`
```cpp
sp<IBinder> ServiceManager::tryGetService(const std::string& name, bool startIfNotFound)
```

这是服务查询逻辑真正的核心函数。

主要流程：

1. 获取调用者上下文
2. 在 `mNameToService` 中查找目标服务
3. 检查隔离进程是否允许访问该服务
4. 检查调用者是否有权限查找该服务
5. 如果服务没找到且 `startIfNotFound=true`，则尝试启动懒加载服务
6. 如果查找成功，则标记 `guaranteeClient = true`

关键点在于：

- `ServiceManager` 本身不执行业务逻辑
- 它只是把对应服务的 `IBinder` 返回出去

### 6.4 `addService`
```cpp
Status ServiceManager::addService(const std::string& name,
                                  const sp<IBinder>& binder,
                                  bool allowIsolated,
                                  int32_t dumpPriority)
```

这是服务注册的核心入口。

主要流程：

1. 获取调用者上下文
2. 检查应用进程不能随意注册系统服务
3. 检查是否有 add 权限
4. 检查 Binder 对象不为空
5. 检查服务名格式是否合法
6. 如果有需要，检查 VINTF 声明
7. 对远程 Binder 执行 `linkToDeath`
8. 将服务写入或覆盖到 `mNameToService[name]`
9. 如果已经存在注册监听者，则通知它们

一句话总结：

`addService` 就是把“服务名 -> Binder 实体”的映射存入 `ServiceManager` 中。

### 6.5 `listServices`
```cpp
Status ServiceManager::listServices(int32_t dumpPriority, std::vector<std::string>* outList)
```

作用：

- 返回符合 `dumpPriority` 条件的服务名称列表

它会先检查列举权限，再遍历 `mNameToService`，把满足优先级条件的服务名导出。

### 6.6 `registerForNotifications`
```cpp
Status ServiceManager::registerForNotifications(
        const std::string& name, const sp<IServiceCallback>& callback)
```

作用：

- 为某个服务名注册通知回调
- 当这个服务后续被注册时，通知监听者

主要流程：

1. 检查调用者是否有权限查找该服务
2. 检查服务名是否合法
3. 检查回调对象不为空
4. 通过 `linkToDeath` 监听回调对象死亡
5. 将回调加入 `mNameToRegistrationCallback[name]`
6. 如果服务当前已经存在，则立即触发一次 `onRegistration`

这是一个典型的“我关心这个服务什么时候出现”的机制。

### 6.7 `binderDied`
```cpp
void ServiceManager::binderDied(const wp<IBinder>& who)
```

作用：

- 处理 Binder 死亡事件
- 将已经失效的服务或回调从管理表中删除

主要动作：

- 从 `mNameToService` 中移除失效 Binder
- 移除失效的注册通知回调
- 移除失效的客户端回调

这对应一个很重要的思想：

```text
ServiceManager 必须保证自己的服务注册表是“活的”和“干净的”
```

### 6.8 `tryStartService`
```cpp
void ServiceManager::tryStartService(const std::string& name)
```

作用：

- 当 `getService` 查不到目标服务时，尝试启动懒加载的 AIDL 服务

代码里是通过：

```cpp
SetProperty("ctl.interface_start", "aidl/" + name)
```

它的含义是：

- 通知 `init` 去启动对应的懒加载服务

这也是 `getService` 和 `checkService` 行为不同的重要原因。

## 7. 一条实用主线
如果你只记住一条主线，就记住下面这段：

```text
服务进程
 -> addService(name, binder)
 -> ServiceManager 记录到 mNameToService

客户端进程
 -> getService/checkService(name)
 -> ServiceManager 返回该服务对应的 binder 对象

如果 binder 死亡
 -> binderDied()
 -> ServiceManager 删除失效记录
```

所以 `ServiceManager.cpp` 的本质，就是维护服务注册表的生命周期和查询过程。

## 8. 和 Binder 驱动的关系
`ServiceManager.cpp` 本身运行在用户空间，它不是驱动代码。

它和 Binder 驱动的关系是：

- Binder 驱动提供 IPC 传输机制
- `ServiceManager` 基于 Binder 机制提供服务管理能力

也就是说：

- Binder 驱动解决“怎么通信”
- `ServiceManager` 解决“谁提供了哪个服务”

## 9. 对 BSP 工程师的阅读重点
对于 BSP 学习来说，不需要一开始把 `ServiceManager.cpp` 每个实现细节都抠完。重点应该放在下面几点：

1. `ServiceManager` 是 Binder 的上下文管理者
2. 系统服务通过 `addService` 向它注册
3. 客户端通过 `getService/checkService` 向它查询服务
4. 返回结果是 Binder 对象，而不是直接执行函数
5. 当 Binder 死亡时，服务需要从注册表中清理掉

只要把这些点串起来，你就能把下面这些关系接上：

- Framework 服务注册
- Binder 通信流程
- HAL / 系统服务启动关系

## 10. 一句话总结
`ServiceManager.cpp` 是 Android Binder 服务的核心用户空间管理者。在 `ServiceManager` 成为 Binder 上下文管理者之后，它负责维护系统服务的注册、查询、通知以及死亡清理。
