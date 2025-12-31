# 快速上手
## 简介
以一段程序引入C语言，具体见`rearrang.c`
- 空白和注释
    如果要从逻辑上删除一段代码，最好的方式是使用条件编译的方式进行注释
    ```c
    #if 0
    // 这段代码将被删除
    #endif
    ```
- 预处理指令
- `main`函数
    每个C程序都必须包含一个`main`函数，它是程序的入口点。`main`函数的返回类型是`int`，它返回一个整数值，表示程序的执行状态。
## 补充说明
## 编译
```bash
gcc xxx.c -o xxx
```
## 总结
## 警告的总结
1. 在`scanf`函数的标量参数前未添加`&`符号，可能会导致程序崩溃。
2. 机械地把`printf`函数的格式代码照搬与`scanf`函数的格式代码，可能会导致程序崩溃。
3. 在应该使用`&&`或`||`运算符时，使用`&`或`|`运算符可能会导致程序崩溃。
4. 误用`=`操作符而不是`==`或`!=`运算符，可能会导致程序崩溃。
## 编程提示的总结
1. 使用`#include`指令避免重复声明
2. 使用`#define`指令给常量值取名
3. 使用`#include`文件中放置函数原型
4. 在使用下标前先检查他们的值
5. 在`while`和`if`表达式中蕴含赋值操作，消除冗余赋值操作
6. 如何编写一个空循环体
7. 始终进行检查，确保指针不为空，数据不越界
## 问题
![Alt text](questions_1_7.png)
1. 规范化书写代码，有利于阅读和修改。  
2. `#include`是预处理指令，把声明放在头文件，方便修改和维护。  
3. `#define`也是预处理指令的一种，见名知意，将变量设置为常量，一般为大写，注意后面没有分号。  
4. `printf("%d %s %f\n", a, b, c);`  
5. `scanf("%d %d %s", &quantity, &price, department); ` 
6. 是为了保持语言的简洁性和的效率，下标检查是程序员自行负责的。  
7. 没具体读，母鸡抖！可能是越界吧。
  英文原答案： `More characters would be copied than are actually needed; however, the output_col would be updated properly, so the next range of characters would be copied into the output array at the proper place, replacing any extra characters from the preceding operation. The only potential problem is that the unbounded strcpy might copy more characters into the output array than it has room to hold, destroying some other variables.`  
8. 如果`gets()`的长度很长，可能会导致数组`input`越界。