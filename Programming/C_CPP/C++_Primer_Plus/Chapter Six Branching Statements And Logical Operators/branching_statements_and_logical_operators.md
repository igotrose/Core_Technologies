# 分支语句和逻辑运算符
## if语句
### if else 语句
### 格式化if else语句
### if else if else 结构
## 逻辑表达式
### 逻辑OR运算符 `||` 
### 逻辑AND运算符 `&&`
### 用`&&`来设置取值范围
### 逻辑NOT运算符 `!`
### 逻辑运算符细节
### 其他表达方式
逻辑运算符：另一种表达式
|运算符|另一种表达式|
|---|---|
|`&&`|`and`|
|`\|\|`|`or`|
|`!`|`not`|
## 字符函数库`cctype` 
`cctype`中的字符函数
|函数名称|返回值|
|-------|------|
|`isalnum()`|如果参数是字符或数字，该函数返回`true`|
|`isalpha()`|如果参数是字母，该函数返回`true`|
|`iscntrl()`|如果参数是控制字符，该函数返回`true`|
|`isdigit()`|如果参数是数字，该函数返回`true`|
|`isgraph()`|如果参数是可见字符，即空格之外的字符，该函数返回`true`|
|`islower()`|如果参数是小写字母，该函数返回`true`|
|`isprint()`|如果参数是可打印字符，包括空格，该函数返回`true`|
|`ispunct()`|如果参数是标点符号，该函数返回`true`|
|`isspace()`|如果参数是空白字符，该函数返回`true`|
|`isupper()`|如果参数是大写字母，该函数返回`true`|
|`isxdigit()`|如果参数是十六进制数字，该函数返回`true`|
|`tolower()`|如果参数是大写字符，则返回小写，否则返回该参数|
|`toupper()`|如果参数是小写字符，则返回大写，否则返回该参数|
## `?:`运算符 （三目运算符）
## switch 语句
### 将枚举量用作标签
### switch 和 if else 
### break 和 continue 语句
### 读取数字的循环
```cpp 
// cingolf.cpp
#include <iostream>
using namespace std;
const int Max = 5;
int main()
{
    int golf[Max];
    cout << "Please enter your golf scores." << endl;
    cout << "You must enter " << Max << " rounds of scores." << endl;
    for (int i = 0; i < Max; i++)
    {
        cout << "Round " << i+1 << ": ";
        while (!(cin >> golf[i]))
        {
            cin.clear();    // clear the error flag
            while (!(cin,get() != '\n'))
                continue;
            cout << "Please enter a number: ";
        }
    }

    double total = 0.0;
    for (int i = 0; i < Max; i++)
        total += golf[i];  
    cout << "Your average score is " << total / Max << endl;
    return 0;
}
```
## 简单文件输入/输出
### 文本I/O和文本文件
### 写入到文本文件中
对于`cout`标准输出流对象，
- 必须包含头文件`iostream`
- 头文件`iostream`定义了一个用处理输出的`ostream`类
- 头文件`iostream`声明了一个名为`cout`的`ostream`对象
- 必须指明名称空间`std`
- 可以结合使用`cout`和运算符`<<`来显示各类型的数据

文件输出与标准输出极其相似
- 必须包含头文件`fstream`
- 头文件`fstream`定义了一个用处理文件输入/输出的`fstream`类
- 头文件`fstream`声明了一个名为`ifstream`和`ofstream`的类
- 必须指明名称空间`std`
- 需要将`ofstream`对象与文件关联起来，为此，方法之一是使用`open()`方法
- 使用完文件后，应使用方法`close()`
- 可结合使用`ofstream`对象和运算符`<<`来输出各种类型的数据

```cpp
// outfile.cpp
#include <iostream>
#include <fstream>

int main()
{
    using namespace std;

    char automobile[50];
    int year;
    double a_price;
    double d_price;

    ofstream outfile;
    outfile.open("cardata.txt");

    cout << "Enter the make and model of automobile: ";
    cin.getline(automobile, 50);
    cout << "Enter the model year: ";
    cin >> year;
    cout << "Enter the original asking price: ";
    cint >> a_price;
    d_price = a_price * 0.913;

    cout << fixed;
    cout.precision(2);
    cout.setf(ios_base::showpoint);
    cout << "Make and model: " << automobile << endl;
    cout << "Year: " << year << endl;
    cout << "Was asking price: $" << a_price << endl;
    
    outFile << fixed;
    outFile.precision(2);
    outFile.setf(ios_base::showpoint);
    outFile << "Make and model: " << automobile << endl;
    outFile << "Year: " << year << endl;
    outFile << "Was asking price: $" << a_price << endl;

    outFile.close();
    return 0;
}
```
`outFile`可以使用`cout`可使用的任何方法
### 读取文本文件
## 总结
## 复习题
## 编程练习