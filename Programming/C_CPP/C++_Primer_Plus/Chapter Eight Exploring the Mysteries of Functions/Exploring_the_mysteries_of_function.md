# 函数探幽
## C++内联函数
内联函数的编译代码与其他程序代码内联起来了，也就是说，编译器将使用相应的函数代码替换函数调用；对于内联代码，程序无需跳到另一个位置处执行代码，再跳回来，在小而简单、频繁调用的函数适当使用内联的特性可以提高程序性能，使用内联特性必须采取下述措施之一
- 在函数声明前加上关键字`inline`
- 在函数定义前加上关键字`inline`
```cpp
// inline.cpp
#include<iostream>

inline double square(double x)
{
    return x * x;
}

int main()
{
    using namespace std;
    double a, b;
    double x = 5;
    a = square(5.0);
    cout << a << endl;
    return  0;
}
```
## 引用变量
C++新增一种复合类型——引用变量，**引用是已定义的变量的别名**，有以下特性
- 必须初始化
- 名称不能改变
- 可以直接访问变量的值
- 无空引用

### 创建引用变量
C和C++使用`&`符号来知识变量的地址，C++赋予了`&`符号一个特殊的功能，即创建引用变量
```cpp
#include<iostream>
using namespace std;
int main()
{ 
    // firstref.cpp
    int rats = 101;
    int & rodents = rats;
    
    cout << "rats = " << rats;
    cout << ", rodents = " << rodents << endl;
    
    cout << "rats address = " << &rats;
    cout << ", rodents address = " << &rodents << endl;
    // secref.cpp 
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
```cpp
// cubes.cpp
#include <iostream>

double cube(double a)
{
    a *= a * a;
    return a;
}

double refcube(double & a)
{
    a *= a * a;     
    return a;
}

int main()
{
    using namespace std;  
    double x = 3.0;
    cout << cube(x) << " = cube of " << x << endl;
    cout << refcube(x) << " = refcube of " << x << endl;
    return 0;
}
```
这里设计到左值和右值的概念，有名字/可取地址的多是左值；临时算出来的多是右值  

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
    // 输出到ep-data.txt，基类的引用可以指向派生类
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
默认参数值得是当函数调用中省略了实参时自动使用的一个值，**这个默认值需要通过函数原型去声明**，但是对于待参数列表的函数，必须是**从右往左添加默认值** 
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
函数模板就是通用的函数描述，他们使用泛型来定义函数，听过将类型作为参数传递给模板，可以使编译器生成该类型的函数，使用函数模板`template <typename AnyType>`，关键字 `template` 和类型名字 `typename` 是必须的，必须使用尖括号 `<>`，模板只是规定了编译器应该怎么做，在由编译器去生成具体的函数定义
```cpp
// funtemp.cpp -- using a function template
#include <iostream> 
// function template prototype
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
需要多个对不同类型使用同一种算法的函数时，可以使用模板，但是并非所有的类型都使用相同的算法，为了满足这种需求，就可以想使用重载常规函数定义一样使用重载模板定义；和常规重载一样，被重载的模板的函数特征标（参数列表）必须不同
```cpp
// twotemps.cpp -- using overloaded template functions
#include <iostream>
template <typename T>   
void Swap(T &a, T &b)       // original template
{
    T temp;
    temp = a;
    a = b;
    b = temp;
}
void Swap(T *a, T *b, int n)    // new template
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
编写的模板函数很可能无法处理某些类型，比如不同类型之间的处理，对于这种局限性，有两种解决方法，一是重载运算符，而是显式具体化，即为特定类型提供具体化的模板定义
### 显式具体化
假设下面提供一个 `job` 结构体，如果用原来的模板，那么大概率是交换两个结构，如果想要单独交换里面的成员则需要使用不同的代码，但是函数的参数保持不变，所以不能使用模板重载来提供具体代码，但是可以提供一个具体化函数定义，其中包含所需的代码，当编译器找到与函数调用匹配的具体化定义，将使用该定义，而不再寻找模板，下面是C++标准定义的形式
1. 对于给定的函数名，可以有非模板函数、模板函数和具体化模板函数以及它们的重载版本
2. 显式具体化的原型和定义应以 `tempalte<>` 打头，并通过名称来指出类型
3. 具体化优先于常规模板，而非模板函数优先于具体化和常规模板

下面是用于交换 `job` 结构的非模板函数、模板函数和具体化模板函数
```cpp
// non template function prototype
void swap(job &j1, job &j2);
// template function prototype
template <typename T>
void Swap(T &a, T &b);
// explicitly specialized template function definition
template <> void Swap<job>(job &j1, job &j2)
```
这里注意 `Swap<job>` 中的 `<job>` 是可选的，因为函数的参数类型表明，这是 `<job>` 的一个具体化
```cpp
// twoswap.cpp -- specialization overrides a template
#include <iostream>

struct job
{
    char name[40];
    double salary;
    int floor;
};

template <typename T>
void Swap(T &a, T &b)
{
    T temp;
    temp = a;
    a = b;
    b = temp;
}
// explicit specialization
template <> void Swap(job &j1, job &j2)
{
    double t1;
    int t2;
    t1 = j1.salary;
    j1.salary = j2.salary;
    j2.salary = t1;
    t2 = j1.floor;
    j1.floor = j2.floor;
    j2.floor = t2;
}

void Show(job &j)
{
    using namespace std;
    cout << j.name << ": $" << j.salary << " on floor " << j.floor << endl;
}

int main()
{
    using namespace std;
    cout.precision(2);
    cout.setf(ios_base::fixed, ios_base::floatfield);
    int i = 10, j = 20;
    cout << "i, j = " << i << ", " << j << ".\n";
    cout << "Using compiler-generated int swapper:" << endl;
    Swap(i, j);
    cout << "Now i, j = " << i << ", " << j << ".\n";

    job sue = {"Susan Yaffee", 7300.60, 7};
    job sidney = {"Sidney Taffee", 7800.60, 9};
    cout << "Before job swapping:" << endl;
    Show(sue);
    Show(sidney);
    Swap(sue, sidney);
    cout << "After job swapping:" << endl;
    Show(sue);
    Show(sidney);
    return 0;
}
```
### 实例化和具体化
模板只是生成函数定义的方案，本身不会生成函数定义，编译器使用模板为特定类型生成函数定义时，得到的是模板实例；比如在下面程序中，函数调用 `Swap(i, j)` 导致编译器生成 `Swap()`的一个示例，该实例使用 `int` 类型，模板并非函数定义，但是使用 `int` 的模板实例是函数定义，这种被称之为**隐式实例化**，但是现在C++还允许**显式实例化**，用 `<>` 符号指定类型，并在声明前加上关键字 `template`，格式如下
```
template void Swap<int>(int, int)
```
### 区别一下
```cpp
template <typename T>
void Swap(T& a, T& b) { /* 主模板 */ }

template <>
void Swap(job& a, job& b) { /* job 特化 */ }

// 显式实例化
template void Swap<int>(int&, int&);
template void Swap<job>(job&, job&);

```

- Swap(ia, ib) → 没特化 → 用主模板实例（T=int）
- Swap(ja, jb) → 有 job 特化 → 用特化版本 
- template void Swap<job>(job&, job&); → 生成的是特化版本，不是主模板 
### 编译器选择使用哪个函数版本
重载解析是指编译器在多个同名函数/模板中，选出一个最合适的的规则过程
1. 创建候选函数列表
2. 使用候选函数列表创建可行函数列表
3. 确定是否有最佳的可行函数

对于重载解析寻找最匹配的函数，有完全匹配和最佳匹配以及部分排序规则；
- 完全匹配和最佳匹配
    进行完全匹配时，C++允许某些无关紧要的转换，如下表
    |从实参|从形参|
    |:----:|:----:|
    | Type | Type & |
    | Type & | Type |
    | Type []| *Type | 
    | Type (argument-list) | Type (*)(argument-list)|
    | Type | const Type |
    | Type | volatile Type |
    | Type* | const Type |
    | Type* | volatile Type * |
- 部分排序规则
    ```cpp
    // tempover.cpp -- template overloading
    #include <iostream>

    using namespace std;
    template <typename T>
    void ShowArray(T arr[], int n)
    {
        cout << "template A \n";
        for (int i = 0; i < n; i++)
            cout << arr[i] << " ";
        cout << endl;
    }

    template <typename T>
    void ShowArray(T * arr[], int n)
    {
        cout << "template B \n";
        for (int i = 0; i < n; i++)
            cout << *arr[i] << " ";
        cout << endl;
    }

    struct debts
    {
        char name[50];
        double amount;
    };

    int main()
    {
        int things[6] = {13, 31, 103, 301, 310, 130};
        struct debts mr_E[3] =
        {
            {"Ima Wolfe", 2400.0},
            {"Curly CM", 1300.0},
            {"Lucy Write", 1900.0}
        };
        double * pd[3];

        for (int i = 0; i < 3; i++)
            pd[i] = &mr_E[i].amount;
        cout << "Listing Mr. E's debt:\n";
        ShowArray(things, 6);
        cout << "Listing Mr. E's array:\n";
        ShowArray(pd, 3);
        return 0;
    }
    ```

重载解析将寻找最匹配的函数
- 如果只存在一个这样的函数，那么就选择它
- 如果存在多个这样的函数，但其中有一个时非模板函数，则选择该函数
- 如果存在多个适合的函数，且他们都为模板函数，但其中有一个函数比其他函数更具体，则选择该函数
- 如果有多个同样合适的非模板函数或模板函数，但没有一个函数比其他函数更具体，那么调用时不确定的，是错误的
- 如果不存在匹配的函数，那也是错误的
### 模板函数的发展
- 关键字 `decltype`，用来推断表达式的类型而不计算表达式本身,可以这样使用该关键字
    ```cpp
    int x;
    decltype(x) y;   // make y the same type as x 
    ```
    给 `decltype` 提供的参数可以是表达式，也可以这样使用
    ```cpp
    decltype(x + y) xpy;   // make xpy the same type as x + y
    xpy = x + y; 
    ```
    在模板和引用中也同样适用

## 总结
## 复习题
## 编程练习