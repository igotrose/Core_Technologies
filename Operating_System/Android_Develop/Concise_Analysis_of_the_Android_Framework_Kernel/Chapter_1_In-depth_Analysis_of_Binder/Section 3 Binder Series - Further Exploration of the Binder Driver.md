# Binder Driver 再探
## Binder 通信简述
Client 进程通过 RPC（Remote Procedure Call Pratocol）与 Server 通信，可以简单的划分为三层：驱动层、IPC层、业务层。`demo()` 是客户端和服务端共同协商好的同意方法；IPC 层的数据包含了 handle、RPC 数据、代码、协议这四项，通过 IPC 层进行数据传输；而真正的客户端和服务端建立通信的基础设施就是 Binder Driver     
![alt text](Binder_communication_architecture_diagram.png)
例如，当名为 `BatteryStatsService` 的客户端项 `ServiceManager` 注册服务的过程中，IPC层的数据组成为：`handle = 0`，RPC代码为 `ADD_SERVICE_TRANSACTION`，RPC数据为 `BatteryStatsService`，Binder协议为 `BC_TRANSACTION`
## Binder 通信协议 
### 通信模型
![alt text](Binder_Communication_Model.png)
## 场景总结
## Binder 内存机制
