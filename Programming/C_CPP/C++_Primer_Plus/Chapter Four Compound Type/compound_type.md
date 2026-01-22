# 复合类型
## 数组
数组声明应当指出一下三点：
1. 存储在每个元素中的值的类型
2. 数组名
3. 数组中的元素数

```cpp
// arrayone.cpp
#include <iostream>

int main() 
{
    using namespace std;
    int yams[3];
    yams[0] = 7;
    yams[1] = 8;
    yams[2] = 6;

    int yamcosts[3] = {20, 30, 5};

    cout << "Total yams = " << yams[0] + yams[1] + yams[2] << endl;
    cout << "The package with " << yams[1] << " yams costs " << yamcosts[1] << " cents per yam.\n";
    int total = yams[0] * yamcosts[0] + yams[1] * yamcosts[1] + yams[2] * yamcosts[2];
    cout << "The total yam expense is " << total << " cents.\n";
    cout << "\nSize of yams array = " << sizeof yams;
    cout << " bytes\nSize of one element = " << sizeof yams[0] << " bytes\n";
    return 0;
} 
```
### 程序说明
`sizeof`用于数组名，得到的将是整个数组中的字节数；`sizeof`用于数组中的元素，得到的是该元素所占的字节数
### 数组的初始化规则
### C++11数组初始化方法
## 字符串
字符串是存储在内存的连续字节中的一系列字符，字符串常量用双引号表示，字符常量用单引号表示
### 拼接字符串常量
有时候字符串很长，无法放到一行中，C++允许拼接字符串字面值，即将两个用引号括起的字符串合并为一个，中间不会自动加空格，只保留一个结尾的 `\0` 比如
```cpp
cout << "I'd give my right arm to be" " a great violionist.\n";  
```
### 在数组中使用字符串
```cpp
// string.cpp
#include <iostream>
#include <cstring>

int main() 
{
    using namespace std;
    const int Size = 15;
    char name1[Size];
    char name2[Size] = "C++owboy";

    cout << "Howdy! I'm " << name2;
    cout << "! What's your name?\n";
    cin >> name1;
    cout << "Well, " << name1 << ", your name has " << strlen(name1) << " letters and is stored\n";
    cout << "in an array of " << sizeof(name1) << " bytes.\n";
    cout << "Your initial is " << name1[0] << ".\n";
    name2[3] = '\0';
    cout << "Here are the first 3 characters of my name: ";
    cout << name2 << endl;

    return 0;
}
```
### 字符串输入
```cpp
// instr1.cpp
#include <iostream>

int main()
{
    using namespace std;
    const int ArSize = 20;
    char name[ArSize];
    char dessert[ArSize];

    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your favorite dessert: \n";
    cin >> dessert;
    cout << "I have some delicius " << dessert;
    cout << " for you, " << name << ".\n";

    return 0;
}
```
`cin`通过使用空白字符（空格、制表符和换行符）来确定字符串的结束位置，在上例中，假设输入有空格，那么空格前的字符会被读取，但是读取完之后会自动在结尾添加空字符，并把后面的输入放当下一个数组
### 每次读取一行字符串输入
1. 面向行的输入：`getline()`，使用通过回车键输入的换行符来确定输入结尾，使用`cin.getline()`，该函数有两个参数，第一个参数为输入的字符串，第二个参数为字符串的长度
    ```cpp
    // instr2.cpp
    #include <iostream>
    int main()
    {
        using namespace std;
        const int ArSize = 20;
        char name[ArSize];
        char dessert[ArSize];

        cout << "Enter your name: ";
        cin.getline(name, ArSize);
        cout << "Enter your favorite dessert: ";
        cin.getline(dessert, ArSize);
        cout << "I have some delicius " << dessert;
        cout << " for you, " << name << "." << endl;

        return 0;
    }
    ```
2. 面向行的输入：`get()`，该成员函数有两种变体，一种变体的工作方式与`getline()`相似，但是不会跨过换行符，另一种变体可以读取一个字符，这里涉及一个函数重载的概念
    ```cpp
    // instr3.cpp
    #include <iostream>
    int main()
    {
        using namespace std;
        const int ArSize = 20;
        char name[ArSize];
        char dessert[ArSize];

        cout << "Enter your name: \n";
        cin.get(name, ArSize).get();
        cout << "Enter your favorite dessert: \n";
        cin.get(dessert, ArSize).get();
        cout << "I have some delicius " << dessert;
        cout << " for you, " << name << "." << endl;

        return 0;
    }
    ```

3. 空行和其他问题
4. 三种输入方式总结
    |输入方式|结束标志|会不会读取换行符|
    |:-----:|:------:|:------------:| 
    |`cin >> x`|空格/回车/EOF|不读|
    |`cin.getline()`|回车/EOF|不读|
    |`cin.get()`|读到指定字符|会读|

### 混合输入字符串和数字
```cpp
// numstr.cpp
#include <iostream>

int main()
{
    using namespace std;
    cout << "What year was your house build?\n";
    int year;
    (cin >> year).get();
    cout << "Year built: " << year << endl;
    cout << "What is its street address?\n";
    char address[80];
    cin.getline(address, 80);
    cout << "Address: " << address << endl;
    cout << "Done!" << endl;
    return 0;
}
```
`cin`读取年份，将会车键生成的换行符留在的输入队列中，后面的`cin.getline()`看到换行符后将起人做空行并赋值给`address`数组，解决方法是在读取地址之前先读取并丢弃换行符
## string 类简介
ISO/ANSI C++标准通过他添加string类扩展了C++库，因此心啊在可以string类型的变量而不是字符数组来存储字符串，使用string类，必须包含string头文件，string类位于名称空间std中，所以必须使用std::string来声明string变量或者提供using声明。
```cpp
// strtype1.cpp
#include <iostream>
#include <string>
int main()
{
    using namespace std;
    char charr1[20];
    char charr2[20] = "jaguar";
    string str1;
    string str2 = "panther";
 
    cout << "Enter a kind of feline: ";
    cin >> charr1;
    cout << "Enter another kind of feline: ";
    cin >> str1;
    cout << "Here are some felines: \n";
    cout << charr1 << " " << charr2 << " " << str1 << " " << str2 << endl;
    cout << "The third letter in " << charr2 << " is " << charr2[2] << endl;
    cout << "The third letter in " << str2 << " is " << str2[2] << endl;

    return 0;
}
```
1. 可以使用C风格字符串来初始化 string 对象
2. 可以使用 `cin` 来讲键盘输入存储到 string 对象
3. 可以使用 `cout` 来显示 string 对象
4. 可以使用数组表示法来的访问存储在 string 对象中的字符

类设计让程序能够自动处理 string 的大小
### C++11字符串初始化
### 赋值、拼接和附加
```cpp
// strtype2.cpp
#include <iostream>
#include <string>

int main()
{
    using namespace std;
    string s1 = "penguin";
    string s2, s3;

    cout << "You can assign one string object to another: s2 = s1\n";
    s2 = s1;
    cout << "s1: " << s1 << ", s2: " << s2 << endl;
    cout << "You can assign a C-style string to a string object.\n";
    cout << "s3 = \"buzzard\"\n";
    s2 = "buzzard";
    cout << "s2: " << s2 << endl;
    cout << "You can append one string to another.\n";
    cout << "s3 = s1 + s2\n";
    s3 = s1 + s2;
    cout << "s3: " << s3 << endl;
    cout << "You can append a C-style string to a string object.\n";
    cout << "s3 += \" leapfrog\"\n";
    s3 += " leapfrog";
    cout << "s3: " << s3 << endl;
    return 0;
}
```
string类简化了字符串合并操作，可以使用运算符`+`将两个string对象合并，还可以使用运算符`+=`来追加字符串
### string类的其他操作
```cpp 
// strtype3.cpp
#include <iostream>
#include <string>
#include <cstring>

int main()
{
    using namespace std;
    char charr1[20];
    char charr2[20] = "jaguar";
    string str1;
    string str2 = "panther";

    str1 = str2;
    strcpy(charr1, charr2);

    str1 += " paste";
    strcat(charr1, " juice");

    int len1 = str1.size();
    int len2 = strlen(charr1);

    cout << "The string " << str1 << " contains " << len1 << " characters.\n";
    cout << "The string " << charr1 << " contains " << len2 << " characters.\n";

    return 0;
}
```
### string类I/O
```cpp
// strtype4.cpp
#include <iostream>
#include <string>
#include <cstring>

int main()
{
    using namespace std;
    char charr[20];
    string str;
    cout << "Length of string in charr before input: " << strlen(charr) << endl;
    cout << "Length of string in str before input: " << str.size() << endl;
    cout << "Enter a string: ";
    cin.getline(charr, 20);
    cout << "You entered: " << charr << endl;
    cout << "Enter another string: ";
    getline(cin, str);
    cout << "You entered: " << str << endl;
    cout << "Length of string in charr after input: " << strlen(charr) << endl;
    cout << "Length of string in str after input: " << str.size() << endl;
    return 0;
}
```
### 其他形式的字符串字面值
## 结构简介
结构是用户定义的类型，结构声明定义了这种类型的数据属性，定义结构描述并标记了能够在结构中的各种数据类型，然后按描述创建结构变量
### 在程序中使用结构
```cpp
// structur.cpp 
#include <iostream>
struct inflatable
{
    char name[20];
    float volume;
    double price;
};

int main()
{
    using namespace std;
    inflatable guest = {"Glorious Gloria", 1.88, 29.99};
    inflatable pal = {"Bambi", 0.5, 21.99};

    cout << "Expand your guest list with " << guest.name << " and " << pal.name << ".\n";
    cout << "You can have both for $" << guest.price + pal.price << "!\n";

    return 0;
}

```
### C++11接口初始化
### 结构可以将 string 类作为成员
### 其他结构属性
```cpp
// assgn_st.cpp
#include <iostream>
struct inflatable
{
    char name[20];
    float volume;
    double price;
};

int main()
{
    using namespace std;
    inflatable bouquet = {"sunflowers", 0.20, 12.49};
    inflatable choice;
    cout << "bouquet: " << bouquet.name << " for $" << bouquet.price << endl;
    
    choice = bouquet;
    cout << "choice: " << choice.name << " for $" << choice.price << endl;
    return 0;
}
```
### 结构数组
```cpp
// arrstruc.cpp
#include <iostream>

struct inflatable
{
    char name[20];
    float volume;
    double price;
};

int main()
{
    using namespace std;
    inflatable guests[2] = 
    {
        {"Bambi", 0.5, 21.99},
        {"Godiva", 0.3, 32.99}
    };
    cout << " guests[0] = " << guests[0].name << " for $" << guests[0].price << endl;
    return 0;
}
```
### 结构中的位字段
与C一样，C++也允许指定占用特定位数的结构成员，这是的创建与某个硬件设备上的寄存器对应的数据结构非常方便。字段的类型应为整型或者枚举，接下来是冒号，冒号后面跟着数字，他指定了使用的位数
```cpp
struct torgle_regiester
{
    unsigned int SN : 4;        // 4 bits for SN value
    unsigned int : 4;           // 4 bits unused;
    bool goodIn : 1;            // valid input 
    bool goodTorgle : 1;        // successful torgling
};
torgle_regiester tr = {14, true, false};
```
## 共用体
多个变量使用同一个存储空间，存储大小是最大的变量所占空间
## 枚举
### 设置枚举量的值
## 指针和自由存储空间
```cpp
// address.cpp
#include <iostream>

int main()
{
    using namespace std;
    int donuts = 6;
    double cups = 4.5;

    cout << "donuts value = " << donuts << " adn donuts address = " << &donuts << endl;
    cout << "cups value = " << cups << " adn cups address = " << &cups << endl;

    return 0;
}
```
指针用于存储值的地址，指针名表示的是地址，`*`运算符被称为解引用操作符，将其应用于指针，可以得到该地址处存储的值
```cpp
// pointer.cpp
#include <iostream>
int main()
{
    using namespace std;
    int updates = 6;
    int * p_updates = &updates;

    cout << "Value: updates = " << updates << ": *p_updates = " << *p_updates << endl;
    cout << "Address: &updates = " << &updates << ": p_updates = " << p_updates << endl;
    *p_updates = *p_updates + 1;
    cout << "Now updates = " << updates << endl;
    return 0;
}
```
### 声明和初始化指针
通常在32位机器上，指针变量的大小为4字节，在64位机器上为8字节
```cpp
// init_ptr.cpp
#include <iostream>
int main()
{
    using namespace std;
    int higgens = 5;
    int * pt = &higgens;

    cout << "Value: higgens = " << higgens << ": *pt = " << *pt << endl;
    cout << "Address: &higgens = " << &higgens << ": pt = " << pt << endl;
    return 0;
}
```
### 指针的危险
一定要在对指针应用解引用之前，将指针初始化位一个确定的适当的地址 
### 指针和数字
### 使用new来分配内存
在C中，可以使用库函数`malloc()`来分配内存，在C++中也可这么做，但是C++提供了`new`运算符，对于一个数据对象，获得并指定分配内存的通用格式如下
```cpp
typeName * pointer_name = new typeName;
```
需要在两个地方指定数据类型，用来指定需要什么样的内存和用来声明合适的指针
```cpp
// use_new.cpp
#include <iostream>

int main()
{
    using namespace std;
    int nights = 1001;
    int * pt = new int;
    *pt = 1001;

    cout << "nights value = ";
    cout << nights << ": location " << &nights << endl;
    cout << "int ";
    cout << "valuee= " << *pt << ": location = " << pt << endl;
    double * pd = new double;
    *pd = 10000001.0;

    cout << "double ";
    cout << "valuee= " << *pd << ": location = " << pd << endl;
    cout << "location of pointer pd: " << &pd << endl;
    cout << "size of pt = " << sizeof(pt) << ": size of *pt = " << sizeof(*pt) << endl;
    cout << "size of pd = " << sizeof(pd) << ": size of *pd = " << sizeof(*pd) << endl;
    return 0;
}
```
### 使用delete来释放内存
`delete`运算符可以在使用完之后将内存归还内存池，使用`delete`时，后面要加上指向内存块的指针
### 使用new来创建动态数组
如果程序只需要一个值，则可能会声明一个变量，因为对于管理一个小型数据对象来说，这样做比使用`new`和指针简单，但是对于大型数据，应该使用`new`；假设要编写一个程序，他是否需要数组取决于用户需求，如果通过声明，那他会在编译的时候就分配了空间，这叫`静态联编`；但是使用`new`，他会在运行阶段创建，这种叫`动态联编`
1. 使用`new`创建动态数组，`new`运算符返回第一个元素的地址，当程序使用完之后使用`delete`去释放内存
    ```cpp
    int * psome = new int [10];
    delete [] psome;
    ```
    `[]` 告诉是程序应当释放整个数组，如果使用`new`的时候，不带方括号，则使用`delete`时也不应该带方括号，因为`new`返回的是指向数组的指针，而不是数组本身；如果使用`new`创建动态数组，那么`delete`时，应该使用`delete []`，为数组分配内存的通用格式如下
    ```cpp
    typeName * pointer_name = new typeName [arraySize];
    ```
    使用`new`和`delete`应当注意以下几点
    - 不要使用`delete`来释放不是`new`分配的内存
    - 不要使用`delete`来释放同一个内存块两次
    - 如果使用`new []`为数组分配内存，则应使用`delete []`来释放
    - 对空指针使用`delete`是安全的
2. 使用动态数组
    ```cpp
    // arraynew.cpp
    #include <iostream>
    
    int main()
    {
        using namespace std;
        double * p3 = new double [3];
        p3[0] = 0.2;
        p3[1] = 0.5;
        p3[2] = 0.8;
        cout << "p3[1] = " << p3[1] << endl;
        p3 = p3 + 1;
        cout << "Now p3[0] is " << p3[0] << endl;
        cout << "p3[1] is " << p3[1] << endl;
        p3 = p3 - 1;;
        delete [] p3;
        return 0;
    } 
    ```
## 指针、数组和指针算术
指针和数组基本等价的原因在于指针算术和C++内部处理数组的方式
```cpp
// addpntrs.cpp
#include <iostream>
int main()
{
    using namespace std;
    double wages[3] = {10000.0, 20000.0, 30000.0};
    short stacks[3] = {3, 2, 1};

    double * pw = wages;
    short * ps = &stacks[0];
    cout << "pw = " << pw << ": *pw = " << *pw << endl;
    pw = pw + 1;
    cout << "add 1 to the pw pointer: " << endl;
    cout << "pw = " << pw << ": *pw = " << *pw << endl;
    cout << "ps = " << ps << ": *ps = " << *ps << endl;
    ps = ps + 1;
    cout << "add 1 to the ps pointer: " << endl;
    cout << "ps = " << ps << ": *ps = " << *ps << endl;

    cout << "access two elements with array notation\n";
    cout << "stacks[0] = " << stacks[0] << ", stacks[1] = " << stacks[1] << endl;
    cout << "access two elements with pointer notation\n";
    cout << "*stacks = " << *stacks << ", *(stacks + 1) = " << *(stacks + 1) << endl;

    cout << sizeof(wages) << " = size of wages array" << endl;
    cout << sizeof(pw) << " = size of pw pointer" << endl;
    return  0;
}
```
### 程序说明
### 指针小结
1. 声明指针
2. 给指针赋值
3. 对指针解引用
4. 区分指针和指针指向的值
5. 数组名
6. 指针算术
7. 数组的动态联编和静态联编
8. 数组表示法和指针表示法
### 指针和字符串
```cpp
// ptrstr.cpp
#include <iostream>
#include <cstring>
int main()
{
    using namespace std;
    char animal[20] = "bear";
    const char * bird = "wren";
    char * ps;

    cout << animal << " and " << bird << "\n";

    cout << "Enter a kind of animal: ";
    cin >> animal;

    ps = animal;
    cout << ps << "!\n";
    cout << "Before using strcpy():\n";
    cout << animal << " at " << (int *) animal << endl;
    cout << ps << " at " << (int *) ps << endl;

    ps = new char[strlen(animal) + 1];
    strcpy(ps, animal);
    cout << "After using strcpy():\n";
    cout << animal << " at " << (int *) animal << endl;
    cout << ps << " at " << (int *) ps << endl;
    delete [] ps;
    return 0;
}
```
一般来说，如果给`cout`提供一个指针，他将打印地址，当如果指针类型为`char *`则输出指向的字符串，如果要显示的字符串的地址，则必须将这种指针强转为另一种指针类型
### 使用new创建动态结构
```cpp
// newstrct.cpp
#include <iostream>

struct inflatable
{
    char name[20];
    float volume;
    double price;
};

int main()
{
    using namespace std;
    inflatable * ps = new inflatable;
    cout << "Enter name of inflatable item: ";
    cin.get(ps->name, 20);
    cout << "Enter volume in cubic feet: ";
    cin >> ps->volume;
    cout << "Enter price: $";  
    cin >> ps->price;
    cout << "Name: " << (ps->name) << endl;
    cout << "volume: " << ps->volume << endl;
    cout << "price: $" << ps->price << endl;
    delete ps;
    return 0;
}
```
1. 一个使用`new`和`delete`的示例
    ```cpp
    // delete.cpp
    #include <iostream>
    #include <cstring>
    using namespace std;
    char * getname()
    {
        char temp[80];
        cout << "Enter last name: ";
        cin >> temp;
        char * pn = new char[strlen(temp) + 1];
        strcpy(pn, temp);
        return pn;
    }
    int main()
    {
        char * name = getname();
        cout << name << " at " << (int *) name << endl;
        delete [] name;

        name = getname();
        cout << name << " at " << (int *) name << endl;
        delete [] name;
        return 0;
    }
    ```
### 自动存储、静态存储和动态存储
1. 自动存储，在程序块内声明的变量，其生命周期和程序块相同，在程序块结束时自动销毁，生命周期由编译器管理，不需要程序员手动释放内存，一般用于局部变量，如`int x = 10;`
2. 静态存储，在程序块外声明的变量，其生命周期和整个程序相同，在程序结束时自动销毁，生命周期由编译器管理，不需要程序员手动释放内存，一般用于全局变量，如`static int x = 10;`
3. 动态存储，在程序块外声明的变量，其生命周期和程序块相同，在程序块结束时需要手动释放内存，一般用于动态分配内存，如`int * p = new int;`
### 类型组合
```cpp
// mixtypes.cpp
#include <iostream>

struct antarctica_year_end
{
    int year;
};

int main()
{
    antarctica_year_end s01, s02, s03;
    s01.year = 1998;
    antarctica_year_end * pa = &s02;
    pa->year = 1999;
    antarctica_year_end trio[3];
    trio[0].year = 2003;
    std::cout << trio->year << std::endl;
    const antarctica_year_end * arp[3] = {&s01, &s02, &s03};
    std::cout << arp[1]->year << std::endl;
    const antarctica_year_end ** ppa = arp;
    auto ppb = arp;
    std::cout << (*ppa)->year << std::endl;
    std::cout << (*(ppa + 1))->year << std::endl;
    return 0;
}
```
## 数组的替代品
### 模板类vector
1. 首先使用`vector`对象，必须包含头文件`vector`
2. 其次，`vector`包含在名称空间`std`中
3. 再则，模板使用不同的语法来指出它存储的数据类型
4. `vector`类使用不同的语法来指定元素数
```cpp
#include <vector>

using namespace std;
// vi 是一个vector<int>对象
vector<int> vi;
int n;
// vd 是一个vector<double>对象，包含n个元素
cin >> n;
vector<double> vd(n);
```
一般而言，使用模板类`vector`格式如下
```cpp
vector<typeName> vt(n_elem);
```
### 模板类array
1. 首先使用`array`对象，必须包含头文件`array`
2. 其次，`array`包含在名称空间`std`中
3. 再则，模板使用不同的语法来指出它存储的数据类型
4. `array`类使用不同的语法来指定元素数和元素的类型
```cpp 
#include <array>

using namespace std;
array<int, 5> ai;
array<double, 4> ad = {1.2, 2.1, 3.43, 4.5};
```
一般而言， 使用模板类`array`格式如下，与`vector`不同的是，`array`的元素数是固定的即`n_elem`不能是变量
```cpp
array<typeName, n_elem> array;
``` 
### 比较数组、vector对象和array对象
都需要注意边界问题
## 总结
### 复习题
### 编程练习