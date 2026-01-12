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
### 将引用用于类对象
将类对象传递给函数时，C++通常的做法是使用引用，例如，可以通过使用引用，让函数将类`string、ostream、istream、ofstream、ifstream`等类的对象作为参数
### 对象、继承和引用
`ostream`是基类，`ofstream`是派生类，由基类继承而来，这就意味着`ofstream`对象可以使用基类的特性    
继承的另一个特征是，基类引用可以指向派生类对象，
```cpp
// filefunc.cpp -- function with ostream & parameter
#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

const int LIMIT = 5;
// os 可以指向ostream对象，也可以指向ofstream对象
void file_it(ostream & os, double fo, const double fe[], int n)
{
    // 是存储这种格式信息所需的数据类型的名称
    ios_base::fmtflags initial;
    // 将对象置于使用顶点表示法模式
    initial = os.setf(ios_base::fixed);
    // precision()指定显示多少位小数
    os.precision(0);
    os << "Focal length of objective: " << fo << "mm\n";
    // 将对象置于显示小数点的模式，即使小数部分为零
    os.setf(ios::showpoint);
    os.precision(1);
    os.width(12);
    os << "f.l. eyepiece";
    os.width(15);
    os << "magnification" << endl;
    for (int i = 0; i < n; i++)
    {
        os.width(12);
        os << fe[i];
        os.width(15);
        os << int(fo / fe[i] + 0.5) << endl;
    }
    os.setf(initial);
    // setf()返回调用它之前有效的所有格式化设置
}

int main()
{
    // 打开文件
    ofstream fout;
    const char * fn = "ep-data.txt";
    fout.open(fn);
    if (!fout.is_open())
    {
        cout << "Can't open " << fn << ". Bye.\n";
        exit(EXIT_FAILURE);
    }
    double objective;
    cout << "Enter the focal length of your "
        "telescope objective in mm: ";
    cin >> objective;
    double eps[LIMIT];
    cout << "Enter the focal lengths, in mm, of " << LIMIT << "eyepieces: \n";
    for (int i = 0; i < LIMIT; i++)
    {
        cout << "Eyepiece #" << i + 1 << ": ";
        cin >> eps[i];
    }
    // 输出到ep-data.txt
    file_it(fout, objective, eps, LIMIT);
    // 输出到终端
    file_it(cout, objective, eps, LIMIT);
    cout << "Done!\n";

    return 0;
}
```
每个对象都存储了自己的格式化设置
### 合适使用引用参数
使用引用参数的主要原因包括：能够修改调用函数中的数据对象和传递引用而不是整个数据对象，一些使用原则
- 对于使用传递的值而不做修改的函数
    - 如果数据对象很小，如内置数据类型或小型结构，则按值传递
    - 如果数据对象是数组，则使用指针
    - 如果数据对象是较大的结构，则使用 const 指针或 const 引用
    - 如果数据对象是类对象，则使用 const 引用
- 对于修改调用函数中数据的函数
    - 如果数据对象是内置数据类型，则使用指针
    - 如果数据对象是数组，则只能使用指针
    - 如果数据对象是结构，则使用指针或引用
    - 如果数据对象是类对象，则使用引用
## 默认参数
默认参数值得是当函数调用中省略了实参时自动使用的一个值，这个默认值需要通过函数原型去声明，但是对于待参数列表的函数，必须时从右往做添加默认值的
```cpp
// left.cpp
#include <iostream>
const int ArSize = 80;

char * left(const char * str, int n = 1)
{
    if (n < 0) n = 0;
    char * p = new char[n + 1];
    int i;
    for (i = 0; i < n && str[i]; i++)
        p[i] = str[i];
    while (i < n)
        p[i++] = '\0';
    p[n] = '\0';
    return p;
}

int main()
{
    using namespace std;
    char sample[ArSize];
    cout << "Enter a string:\n";
    cin.get(sample, ArSize);
    char *ps = left(sample, 4);
    cout << ps << endl;
    delete [] ps;
    ps = left(sample);
    cout << ps << endl;
    delete [] ps;
    return 0;
}
```
## 函数重载
函数多台（重载）可以有多个同名的函数但是功能不一样，函数重载的关键是函数的参数列表 —— 也成为函数特征标，C++通过这个识别参数列表来确定调用什么函数，又有点类型引用和类型本身会被视作同一个特征标
### 重载示例
```cpp
// leftover.cpp
#include <iostream>
const int ArSize = 80;

char * left(const char * str, int n = 1)
{
    if (n < 0) n = 0;
    char * p = new char[n + 1];
    int i;
    for (i = 0; i < n && str[i]; i++)
        p[i] = str[i];
    while (i < n)
        p[i++] = '\0';
    p[n] = '\0';
    return p;
}

unsigned long left(unsigned long num, unsigned ct)
{
    unsigned digits = 1;
    unsigned long n = num;

    if(ct == 0 || num == 0)
        return 0;
    while (n /= 10)
        digits++;
    if (digits > ct)
    {
        ct = digits - ct;
        while (ct--)
            num /= 10;
        return num;
    }
    else
        return num;
}

int main()
{
    using namespace std;
    const char * trip = "Hawaii!!";
    unsigned long n = 12345678;
    int i;
    char * temp;

    for (i = 1; i < 10; i++)
    {
        cout << left(n, i) << endl;
        temp = left(trip, i);
        cout << temp << endl;
        delete [] temp;
    }
    return 0;
}
```
### 合适使用函数重载
## 函数模板
函数模板就是通用的函数描述，他们使用泛型来定义函数，听过将类型作为参数传递给模板，可以使编译器生成该类型的函数，使用函数模板`template <typename AnyType>`，关键字和类型名字是必须的
```cpp
// funtemp.cpp
#include <iostream>

template <typename T>
void Swap(T &a, T &b)
{
    T temp;
    temp = a;
    a = b;
    b = temp;
}

int main()
{
    using namespace std;
    int i = 10;
    int j = 20;
    cout << "i, j: " << i << ", " << j << endl;
    cout << "Using compiler-generated int swapper:" << endl;
    Swap(i, j);
    cout << "Now i, j: " << i << ", " << j << endl;

    double x = 24.5;
    double y = 81.7;
    cout << "x, y: " << x << ", " << y << endl;
    cout << "Using compiler-generated double swapper:" << endl;
    Swap(x, y);
    cout << "Now x, y: " << x << ", " << y << endl;
    return 0;
}
```
### 重载的模板
```cpp
// twotemps.cpp
#include <iostream>
template <typename T>
void Swap(T &a, T &b)
{
    T temp;
    temp = a;
    a = b;
    b = temp;
}
void Swap(T *a, T *b, int n)
{
    T temp;
    for (int i = 0; i < n; i++)
    {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}

void Show(int a[])
{
    using namespace std;
    cout << a[0] << a[1] << "/";
    cout << a[2] << a[3] << "/";
    for (int i = 3; i < 5; i++)
        cout << a[i] << " ";
    cout << endl;
}
const int Lim = 8;
int main()
{
    using namespace std;
    int i = 10, j = 20;
    cout << "i, j = " << " << i << ", " << j << endl";
    cout << "Using compiler-generated int swapper:" << endl;
    Swap(i, j);
    cout << "Now i, j = " << i << ", " << j << endl;

    int d1[Lim] = {0, 7, 0, 4, 1, 7, 7, 6};
    int d2[Lim] = {0, 7, 2, 0, 1, 8, 9, 8};
    cout << "Original arrays:" << endl;
    Show(d1);
    Show(d2);
    Swap(d1, d2, Lim);
    cout << "Swapped arrays:" << endl;
    Show(d1);
    Show(d2);
    return 0;
}

```
### 模板的局限性
### 显示具体化
### 实例化和具体化
### 模板函数的发展
## 总结
## 复习题
## 编程练习