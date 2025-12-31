# 函数 —— C++的编程模块
## 复习函数的基本知识
使用C++函数，必须完成如下工作
- 提供函数定义
- 提供函数原型
- 调用函数
### 定义函数
### 函数原型和函数调用
## 函数参数和按值传递
### 多个参数
### 另外一个接收两个参数的函数
## 函数和数组
### 函数如何使用指针来处理数组
### 将数组作为参数意味着什么
### 更多数组函数示例
### 使用数组区间的函数
### 指针和const
## 函数和二维数组
## 函数和C-风格字符串
### 将C-风格字符串作为参数的函数
### 返回C-风格字符串的函数
## 函数和结构
### 传递和返回结构
### 另一个处理结构的函数示例
### 传递结构的地址
## 函数和String对象
```cpp
// topfive.cpp
#include <iostream>
#include <string>

using namespace std;

const int SIZE = 5;
void display (const string sa[], int n);
int main()
{
    string list[SIZE];
    cout << "Enter your " << SIZE << " favorite astronmical sights: " << endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << i + 1 << ": ";
        getline(cin, list[i]);
    }
    cout << "Your list: " << endl;
    diusplay(list, SIZE);
    
    return 0;
}

void display (const string sa[], int n)
{
    for (int i = 0; i < n; i++)
        cout << i + 1 << ": " << sa[i] << endl;
}
```
## 函数与array对象
```cpp
// arrobj.cpp
#include <iostream>
#include <array>
#include <string>

const int Seasons = 4;
const std::array<std::string, Seasons> Snames = {"Spring", "Summer", "Fall", "Winter"};
void fill(std::array<double, Seasons> * pa);
void show(std::array<double, Seasons> da);

int main()
{
    std::array<double, Seasons> expenses;
    fill(&expenses);
    show(expenses);
    return  0;
}

void fill(std::array<double, Seasons> * pa)
{
    using namespace std;
    for (int i = 0; i < Seasons; i++)
    {
        cout << "Enter " << Snames[i] << " expenses: ";
        cin >> (*pa)[i];
    }
}

void show(std::array<double, Seasons> da)
{
    using namespace std;
    double total = 0.0;
    for (int i = 0; i < Seasons; i++)
    {
        cout << Snames[i] << " expenses: " << da[i] << endl;
        total += da[i];
    }
    cout << "Total expenses: " << total << endl;
}
```
## 递归
### 包含一个递归调用的递归
### 包含多个递归调用的递归
```cpp
// ruler.cpp
#include <iostream>
const int Len = 66;
const int Dive = 6;

void subdivide(char ar[], int low, int high, int level);
int main()
{
    char ruler[Len];
    int i;
    for (i = 0; i < Len; i++)
        ruler[i] = '-';
    ruler[Len - 1] = '\0';
    int max = Len -2;
    int min = 0;
    ruler[min] = ruler[max] = '|';
    std::cout << "The ruler is: " << ruler << std::endl;
    for (i = 1; i < Dive; i++)
    {
        subdivide(ruler, min, max, i);
        std::cout << "The ruler is: " << ruler << std::endl;
        for (int j = 0; j < Len; j++)
            ruler[j] = ' ';
    }
    return 0;
}

void subdivide(char ar[], int low, int high, int level)
{
    if (level == 0)
        return;
    int mid = (low + high) / 2;
    ar[mid] = '|';
    subdivide(ar, low, mid - 1, level - 1);
    subdivide(ar, mid + 1, high, level - 1);
}
```
## 函数指针
### 函数指针的基础知识
1. 获取函数的地址
2. 声明函数指针
3. 使用指针来调用函数
### 深入讨论函数指针
### 使用typedef进行简化
## 总结
## 复习题
## 编程练习
