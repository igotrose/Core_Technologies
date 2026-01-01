# 开始学习C++
## 进入C++
```cpp
// myfirst.cpp 
#include <iostream>

int main() 
{
    using namespace std;
    cout << "Come up and C++ me some time!";
    cout << endl;
    cout << "You won't regret it!" << endl;
    return 0;
}
```
### main()函数
### C++注释
### C++预处理器和iostream文件
使用cin和cout必须包含iostream文件
### 头文件名
### 名称空间
名称空间支持是C++的的一个重要特性，C++中的所有函数都包含在名称空间中，核心思想是向标识符等放入不同的区域中，使同名标识符在不同区域内互不干扰
### 使用cout进行C++输出
```cpp
cout << "Hello, world!" << endl;
```
- `cout`是一个预定义对象，知道如何显示字符串、数字和单个字符等，从C++上去理解，输出是一个流，cout对象表示这种流
- `<<`是C++的插入运算符，他是`cout`的一种对象属性，在C中这是左移运算符，在C++中，这是运算符重载
- `endl`是一个预定义对象，表示一个换行符
### C++源代码的格式化
## C++语句
C++程序是一组函数，每个函数又是一组语句，下面的代码主要是声明语句创建变量，赋值语句
```cpp
// carrots.cpp
#include <iostream>

int main()
{
    using namespace std;

    int carrots = 25;

    cout << "I have"
    cout << carrots; 
    cout << " carrots.";
    cout << endl;
    carrots = carrots - 1;
    cout << "Crunch, crunch. Now I have" << carrots << " carrots." << endl;
    return 0;
} 
```
### 声明语句和变量
### 赋值语句
### cout的新花样
cout智能对象
## 其他C++语句
```cpp
// carrots.cpp
#include <iostream>

int main()
{
    using namespace std;

    int carrots;
    cout << "How many carrots do you have?" << endl;
    cin >> carrots;
    cout << "Here are two more"
    cout << carrots;
    carrots = carrots + 2;
    cout << "Now I have" << carrots << " carrots." << endl;
    return 0;
}
```
### 使用cin
cin也是智能对象
### 使用cout进行拼接
### 类简介
类是用于定义的一种数据类型，类定义描述的是数据格式机器用法，而对象则是根据数据格式规范创建的实体
## 函数
### 使用有返回值的函数
```cpp
// sqrt.cpp
#include <iostrem>
#include <cmath>

int main ()
{
    using namespace std;

    double area;
    cout << "Enter the floor area, in square feet, of your home: ";
    cin >> area;
    double side = sqrt(area);
    cout << "That's the equivalent of a square " << side << " feet to the side." << endl;
    cout << "How fascinating!" << endl;
    return 0;
}
```
### 函数变体
指通过不同方式实现的具有相同名称或者相似功能的多个函数版本，主要包括函数重载，模板特化，函数模板实例化，虚函数的多态变体
### 用户自定义函数
```cpp
// ourfunc.cpp
#include <iostream>
void simon(int);

int main()
{
    using namespace std;
    simon(3);
    cout << "Pick an integer";
    int count;
    cin >> count;
    simon(count);
    cout << "Done!" << endl;
    return 0;
}

void simon(int n)
{
    using namespace std;
    cout << "Simon says touch your face " << n << " times";
}
```
### 用户定义的有返回值的函数
```cpp
// convert.cpp
#include <iostream>
int stonetolb(int);
int main()
{
    using namespace std;
    int stone; 
    cout << "Enter the weight in stone: ";
    cin >> stone;
    int pounds = stonetolb(stone);
    cout << stone << " stone = " << pounds << " pounds." << endl;
    return 0;
}
int stonetolb(int sts)
{
    return 14 * sts;
}
```
### 在多函数程序中使用using编译指令
```cpp
// ourfunc1.cpp
#include <iostream>

using namespace std;
void simon(int n);

int main()
{
    simon(3);
    cout << "Pick an integer";
    int count;
    simon(count);
    cout << "Done!" << endl;
    return 0;
}

void simon(int n)
{
    cout << "Simon says touch your face " << n << " times";
}
```
## 总结
## 复习题
1. 函数
2. 编译的时候包含iostream文件的内容
3. 指定std名称空间
4. `cout << "Hello, world!" << endl;`
5. `int cheeses;`
6. `cheeses = 32;`
7. `cin >> cheeses;`
8. `cout << "We have" << cheeses << "varities of cheeses." << endl;`
9. 略
10. 当函数的返回类型为`void`的时候
11. 纠正
    ```cpp
    #include <iostream>
    using namespace std;
    int main()
    {
        char PIN[10];
        cout << "Please enter your PIN: ";
        cin >> PIN;
        return 0;
    }
    ```
## 编程练习