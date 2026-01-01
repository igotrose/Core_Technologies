# 处理数据
## 简单变量
### 变量名
### 整型
### 整形short、int、long和longlong
```cpp
#include <iostream>
#include <climits>

int main() 
{
    using namespace std;
    int n_int = INT_MAX;
    short n_short = SHRT_MAX;
    long n_long = LONG_MAX;
    long long n_longlong = LLONG_MAX;

    cout << "int is " << sizeof(int) << " bytes.\n"; << endl;
    cout << "short is " << sizeof(short) << " bytes.\n"; << endl;
    cout << "long is " << sizeof(long) << " bytes.\n"; << endl;
    cout << "long long is " << sizeof(long long) << " bytes.\n"; << endl;
    
    cout << "Maximum values:\n";
    cout << "int: " << n_int << endl;
    cout << "short: " << n_short << endl;
    cout << "long: " << n_long << endl;
    cout << "long long: " << n_longlong << endl;

    cout << "Minimum values:\n";
    cout << "int: " << INT_MIN << endl;
    cout << "short: " << SHRT_MIN << endl;
    cout << "long: " << LONG_MIN << endl;
    cout << "long long: " << LLONG_MIN << endl;

    cout << "Bits per byte = : " << CHAE_BIT << endl;

    return 0;
}
```
### 无符号类型
```cpp
// exceed.cpp
#include <iostream>
#define ZERO 0
#include <climits>

int main() 
{
    using namespace std;
    short sam = SHRT_MAX;
    unsigned short sue = sam;   

    cout << "Sam has " << sam << "dollars and Sue has " << sue;
    cout << " dollars deposited.\n" << endl << "Add $1 to each account." << endl << "Now ";
    sam = sam + 1;
    sue = sue + 1;
    cout << "Sam has " << sam << " dollars and Sue has " << sue << " dollars deposited.\n";

    sam = ZERO;
    sue = ZERO;
    cout << "Sam has " << sam << " dollars and Sue has " << sue << " dollars deposited.\n";

    sam = sam - 1;
    sue = sue - 1;
    cout << "Sam has " << sam << " dollars and Sue has " << sue << " dollars deposited.\n";

    return 0;
}
```
### 选择整型类型
### 整型字面值
```cpp
// hexoctl.cpp
#include <iostream>

int main() 
{
    using namespace std;
    int chest = 42;
    int waist = 0x42;
    int inseam = 042;

    cout << "Monsieur cuts a striking figure!\n";
    cout << "Chest: " << chest << "{42 in decimal}\n"; 
    cout << "Waist: " << waist << "{42 in hex}\n";
    cout << "Inseam: " << inseam << "{42 in octal}\n";

    return 0;
}
```
```cpp
// hexoct2.cpp
#include <iostream>

int main() 
{
    using namespace std;
    int chest = 42;
    int waist = 42;
    int inseam = 42;

    cout << "Monsieur cuts a striking figure!\n";
    cout << "Chest: " << chest << "{42 in decimal}\n"; 
    cout << hex;
    cout << "Waist: " << waist << "{42 in hex}\n";
    cout << oct;
    cout << "Inseam: " << inseam << "{42 in octal}\n";

    return 0;
}
```
### C++如何确定常量的类型
### char类型：字符和小整数
```cpp
// chartype.cpp
#include <iostream>
int main() 
{
    using namespace std;
    char ch;
    cout << "Enter a character: " << endl;
    cin >> ch;
    cout << "Hola! " << ch << " is a character." << endl;
    return 0;
}
```
```cpp
// morechar.cpp
#include <iostream>
int main() 
{
    using namespace std;
    char ch = 'M';
    int i = ch;
    cout << "The ASCII code for " << ch << " is " << i << endl;
    cout << "Add one to the character code: " << endl;
    ch = ch + 1;
    i = ch;
    cout << "The ASCII code for " << ch << " is " << i << endl;
    cout << "Displaying char ch using a cast: " << endl;
    cout << "The ASCII code for " << (char) ch << " is " << ch << endl;
    cout << "Done" << endl;
    return 0;
}
```
成员函数描述了操纵类数据的方法
### bool类型
## const限定符
## 浮点数
### 书写浮点数
### 浮点类型
### 浮点常量
### 浮点数的优缺点
## C++算术运算符
### 运算符优先级和结合性
### 除法分支
### 求模运算符
### 类型转换
1. 初始化和复制进行转换
    ```cpp
    // assign.cpp
    #include <iostream>

    int main() 
    {
        using namespace std;
        float tree = 3;
        int guess{3.9832};
        int debt = 7.2E12;
        cout << "tree = " << tree << endl;
        cout << "guess = " << guess << endl;
        cout << "debt = " << debt << endl;
        return 0;
    }

    tree = 3.000000
    guess = 3
    debt = 1634811904
    ```
2. 以`{}`方式初始化时进行的转换
    在C++11之后，将使用大括号的初始化称为列表初始化，列表初始化不允许窄化转换，简单理解就是不允许把大盒子的东西往小盒子塞，可能会塞不下或丢东西
    ```cpp
    const int code = 66;
    int x = 66;
    char c1 {66};
    char c2 = {x}; // not allowed
    ```
3. 表达式中的转换，隐式转换，整型提升
4. 传递参数时的转换
5. 强制类型转换
    ```cpp
    // typecast.cpp
    #include <iostream>
    int main() 
    {
        using namespace std;
        int auks, bate, coots;

        auks = 19.99 + 11.99;
        bate = (int)19.99 + (int)11.99;     // old c syntax
        coots = int(19.99) + int(11.99);    // new cpp syntax   
        cout << "auks = " << auks << ", bate = " << bate << ", coots = " << coots << endl;

        char ch = 'Z';
        cout << "The code for " << ch << " is " << int(ch) << endl;
        cout << "Yes, the code is " << static_cast<int>(ch) << endl;
        // using static_cast

        return 0;
    }
    ```
6. C++11中的`auto`声明
## 总结
## 复习题
1. 多种整型类型可以根据特定需求选择最适合的类型
2. ```cpp
    short rbis = 80;
    unsigned int q = 32110;
    unsigned long ants = 3000000000;
    ```
3. C++没有提供自动防止超出整型限制的功能，但是可以使用`climits`来确定限制情况
4. 33L是长整型，33是整型
5. 不等皆，是某种情况下是等效的
6. ```cpp
    char c = 88;
    cout << c << endl;

    cout.put(char(88));

    cout << char{88} << endl;

    cout << {char}88 << endl;
    ```
7. 如果long的长度是4个字节，那就没有损失，但是long long给到double就会有损失
8. 74、4、0、4.5、3
9. int pos = {int}x1 + {int}x2;
10. int、float、char、char、double
## 编程练习