# 函数探幽
## C++内联函数
内联函数的编译代码与其他程序代码内联起来了，也就是说，编译器将使用相应的函数代码替换函数调用；对于内联代码，程序无需跳到另一个位置处执行代码，再跳回来，在小而简单、频繁调用的函数适当使用内联的特性可以提高程序性能，使用内联特性必须采取下述措施之一
- 在函数声明前加上关键字`inline`
- 在函数定义前加上关键字`inline`
## 引用变量
C++新增一种复合类型——引用变量，引用是已定义的变量的别名，有以下特性
- 必须初始化
- 名称不能改变
- 可以直接访问变量的值
- 无空引用

### 创建引用变量
C和C++使用`&`符号来知识变量的地址，C++赋予了`&`符号一个特殊的功能，即创建引用变量
```cpp
// secref.cpp 
#include<iostream>
using namespace std;
int main()
{
    int rats = 101;
    int & rodents = rats;
    
    cout << "rats = " << rats;
    cout << ", rodents = " << rodents << endl;
    
    cout << "rats address = " << &rats;
    cout << ", rodents address = " << &rodents << endl;
    
    int bunnies = 50;
    rodents = bunnies;
    cout << "bunnies = " << bunnies;
    cout << ", rats = " << rats;
    cout << ", rodents = " << rodents << endl;
    
    cout << "bunnies address = " << & bunnies;
    cout << ", rodents address = " << &rodents << endl;
    
    return 0;
}
```
- 引用变量必须立刻绑定，共享同一地址
- 可以改变引用变量的值，但是不能改变引用变量的绑定
**引用变量必须在声明时进行初始化**
### 将引用用作函数参数
传递引用和传递指针都可以改变变量的值
```cpp
// swaps.cpp
#include <iostream>

void swapr(int & a, int & b)
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

void swapp(int * a, int * b)
{
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void swapv(int a, int b)        // swap failed, can't modify value 1
{
    int temp;
    temp = a;
    a = b;
    b = temp;
}

int main()
{
    using namespace std;
    int wallet1 = 300;
    int wallet2 = 350;

    cout << "wallet1 = $" << wallet1;
    cout << ", wallet2 = $" << wallet2 << endl;

    cout << "Using references to swap contents:" << endl;
    swapr(wallet1, wallet2);
    cout << "wallet1 = $" << wallet1;
    cout << ", wallet2 = $" << wallet2 << endl;

    cout << "Using pointers to swap contents:" << endl;
    swapp(&wallet1, &wallet2);
    cout << "wallet1 = $" << wallet1;
    cout << ", wallet2 = $" << wallet2 << endl;

    cout << "Trying to use passing by value:\n";
    swapv(wallet1, wallet2);
    cout << "wallet1 = $" << wallet1;
    cout << ", wallet2 = $" << wallet2 << endl;

    return 0;
}
```
### 引用的属性和特别之处
### 将引用用于结构
```cpp
// strc_ref.cpp
#include <iostream>
#include <string>

using namespace std;
struct free_throws{
    string name;
    int made;
    int attempts;
    float percent;
};

void display(const free_throws & ft);
void set_pc(free_throws & ft);
free_throws & accumulate(free_throws & target, const free_throws & source);

int main()
{
    free_throws one = {"Ifelsa Branch",13,14};
    free_throws two = {"Bulls-Eye",12,13};
    free_throws three = {"Miss Moose",10,10};
    free_throws four = {"Lucky Luciano",7,9};
    free_throws five = {"Injured Miss Moose",6,7};
    free_throws team = {"Throwgoods",0,0};

    free_throws dup;

    set_pc(one);
    display(one);
    accumulate(team, one);
    display(team);

    display(accumulate(team, two));
    accumulate(accumulate(team, three), four);
    display(team);

    dup = accumulate(team, five);
    cout << "Displaying team:" << endl;
    display(team);

    cout << "Displaying dup after assignment:" << endl;
    display(dup);
    set_pc(four);

    accumulate(dup, five) = four;
    std::cout << "Displaying dup after ill-advised assignment:" << endl;
    display(dup);

    return  0;
}

void display(const free_throws & ft)
{
    cout    << "Name: " << ft.name << "\n";
    cout    << "Made: " << ft.made << "\n";
    cout    << "Attempts: " << ft.attempts << "\n";
    cout    << "Percent: " << ft.percent << "\n";
}

void set_pc(free_throws & ft)
{
    if(ft.attempts != 0)
        ft.percent = (float)ft.made / ft.attempts * 100;
    else
        ft.percent = 0;
}

free_throws & accumulate(free_throws & target, const free_throws & source)
{
    target.attempts += source.attempts;
    target.made += source.made;
    set_pc(target);
    return target;
}
```
 