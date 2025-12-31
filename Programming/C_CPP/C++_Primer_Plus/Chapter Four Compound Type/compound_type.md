# 复合类型
## 数组
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
### 数组的初始化规则
### C++11数组初始化方法
## 字符串
### 拼接字符串常量
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
    coutt << "in an array of " << sizeof(name1) << " bytes.\n";
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
    cout << "Enter your favorite dessert: ";
    cin >> dessert;
    cout << "I have some delicius " << dessert;
    cout << " for you, " << name << ".\n";

    return 0;
}
```
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

        cout << "Enter your name: "
        cin.getline(name, ArSize);
        cout << "Enter your favorite dessert: "
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
### 混合输入字符串和数字
```cpp
// numstr.cpp
#include <iostream>

int main()
{
    using namespace std;
    cout << "What year was your house build?\n";
    int year;
    cin >> year;
    cout << "Year built: " << year << endl;
    cout << "What is its street address?\n";
    char address[80];
    cin.getline(address, 80);
    cout << "Address: " << address << endl;
    cout << "Done!" << endl;
    return 0;
}
```
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
 
    cout << "Enter a kind of feline: "
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
类设计让程序能够自动处理string的大小
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
    strcpy(charr1, " juice");

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
    cin, getline(charr, 20);
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
### 在程序中使用结构
### C++11接口初始化
### 其他结构属性
### 结构数组
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
## 枚举
### 设置枚举量的值
## 指针和自由存储空间
### 声明和初始化指针
### 指针的危险
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
    int  * pt = new int;
    *pt = 1001;

    cout << "nights value = ";
    cout << nights << ": location " << &nights << endl;
    cout << "int ";
    cout << "valuee= " << *pt << ": location = " << pt << endl;
    double * pd = new double;
    *pdd = 10000001.0;

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
    如果使用`new`的时候，不带方括号，则使用`delete`时也不应该带方括号，因为`new`返回的是指向数组的指针，而不是数组本身；如果使用`new`创建动态数组，那么`delete`时，应该使用`delete []`，为数组分配内存的通用格式如下
    ```cpp
    typeName * pointer_name = new typeName [arraySize];
    ```
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
        cout << "p3[1] is" << p3[1] << endl;
        p3 = p3 - 1;;
        delete [] p3;
        return 0;
    } 
    ```
## 指针、数组和指针算术
### 指针小结
### 指针和字符串
### 使用new创建动态结构
### 自动存储、静态存储和动态存储
1. 自动存储，在程序块内声明的变量，其生命周期和程序块相同，在程序块结束时自动销毁，生命周期由编译器管理，不需要程序员手动释放内存，一般用于局部变量，如`int x = 10;`
2. 静态存储，在程序块外声明的变量，其生命周期和程序块相同，在程序块结束时自动销毁，生命周期由编译器管理，不需要程序员手动释放内存，一般用于全局变量，如`static int x = 10;`
3. 动态存储，在程序块外声明的变量，其生命周期和程序块相同，在程序块结束时需要手动释放内存，一般用于动态分配内存，如`int * p = new int;`
### 类型组合
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