# 信号量管理
之前的队列，可以用于传递数据，也可以传递状态，但是在有些时候我们只需要维护一个数值一个状态，这死后心凉的效率就会更高更节省空间

## 信号量特性
### 信号量的常规操作
常规操作就是对信号量进行加1或者减1，但是信号量的加1和减1操作是有限制的，必须满足一定条件才可以进行加1或者减1操作，二值信号量和计数信号量都有加1和减1操作，但是二值信号量只能加1或者减1，不能为负数，计数信号量可以为负数
![alt text](semaphore_operation.png)
### 信号量跟队列对比
|队列|信号量|
|---|---|
|可以容纳多个数据，创建队列时有两部分内存：队列结构体和存储数据空间|只能计数值，创建信号量时需要分配信号量结构体|
|生产者：没有空间存取数据可以阻塞|生产者：用于不阻塞，计数值已经达到最大时返回失败|
## 信号量函数
### 创建
- 二值信号量创建
    ```c
    SemaphoreHandle_t xSemaphoreCreateBinary(void);
    SemaphoreHandle_t xSemaphoreCreateBinaryStatic( StaticSemaphore_t *pxSemaphoreBuffer );
    ```
- 计数信号量创建
    ```c
    SemaphoreHandle_t xSemaphoreCreateCounting( uint32_t uxMaxCount, uint32_t uxInitialCount );
    SemaphoreHandle_t xSemaphoreCreateCountingStatic( uint32_t uxMaxCount, uint32_t uxInitialCount, StaticSemaphore_t *pxSemaphoreBuffer );
    ```
### 删除
```c
void vSemaphoreDelete( SemaphoreHandle_t xSemaphore );
```
### give/take
- 在任务中使用
    ```c
    BaseType_t xSemaphoreGive( SemaphoreHandle_t xSemaphore );
    BaseType_t xSemaphoreTake( SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait );
    ```
- 在中断中使用
    ```c
    void vSemaphoreGiveFromISR( SemaphoreHandle_t xSemaphore, BaseType_t pxHigherPriorityTaskWoken );
    BaseType_t xSemaphoreTakeFromISR( SemaphoreHandle_t xSemaphore, BaseType_t  pxHigherPriorityTaskWoken );
    ```