# 语句
## 空语句
空语句本神只包含一个分号，表示什么都不做。
## 表达式语句
## 代码块
代码块就是位于一对花括号之内的可选的声明和语句列表。
## `if`语句
### 语法格式：
```c
if( expression )
    statement;
else  
    statement;
```
`else`子句遵循就近原则，将匹配与它最近的`if`语句
## `while`语句
```c
while(expression)
    statement;
```
### `break`和`continue`语句
`break`语句用于终止循环，`continue`语句用于跳过当前循环。
### `while`语句的循环过程
## `for`语句
```c
for ( expression1; expression2; expression3 )
    statement;
```
其中`statement`是循环体；`expression1`是初始化部分，`expression2`是条件部分，`expression3`是调整部分。
## `do-while`语句
```c
do
    statement;
while( expression );
```
do语句至少执行一次循环！
## switch语句
```c
switch (expression)
{
    case constant-expression:
        statement;
        break; 
    default:
        statement;
        break;
}
```
`expression`必须是整型变量
### `switch`语句中的`break`语句
### `default`子句
建议在每个switch语句末尾都加上default子句。
### `switch`语句的执行流程
## `goto`语句
```c
goto 语句标签；
```
在多层循环嵌套中使用`goto`语句可以跳出多层循环，在做错误判断时使用`goto`语句可以提高程序的可读性。但大多数情况下不建议使用`goto`语句。
## 总结
## 警告的总结
1. 编写不会产生任何结果的表达式
2. 确幸在`if`语句中的语句列表前后加上花括号
3. 在`switch`语句中，执行流意外地从一个`case`顺延到下一个`case`
## 编程提示的总结
1. 在一个没有循环体的循环中，用一个分号表示空语句，并让他独占一行
2. `for`循环的可读性比`while`循环好
3. 在每个`switch`语句末尾加上`default`子句，并在每个`case`语句末尾加上`break`语句
## 问题
![alt text](question_4_13_1.png)
![alt text](question_4_13_2.png) 
![alt text](question_4_13_3.png)
1. 合法，它在执行$3x^2 - 4x + 6$ 则一条算式。  
2. `variable = expression;`
3. 合法的，可能用过，但不一定想过。  
4. 若`then`子句没有语句，可以使用空语句占位。将条件取反，使用一个if语句即可。  
5. 打印`0~9`.
6. 在没有初始化和调整时，`while`语句比较合适。
7. 对个语句需要用花括号进行成块处理。  
8. 需要先执行一次在进行判断时，选用`do`语句较为合适。  
9. `odd、even、odd、odd、even、odd`，`case`执行完没有`break`  
10. 见代码
```c
#include<stdio.h>
int main()
{
    int i, n;
    scanf("%d\n", &n);
    while(i <= n)
    {
        printf("\n");
        i++;
    }
    return 0;
}
```   
11. 见代码
```c
#include <stdio.h>
int main() {
    int x, y, a, b;
    scanf("%d %d %d %d", &x, &y, &a, &b);
    
    if (x < y || a >= b)
        puts("WRONG");
    else
        puts("RIGHT");
    return 0;
}
```
12. 见代码
```c
#include<stdio.h>
int main()
{
    int year, leap_year;
    printf("please input a year:->\n");
    scanf("%d\n", &year);
    if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0)
    {
        leap_year = 1;
    }
    else
    {
        leap_year = 0;
    }
    printf("%d\n", leap_year);
    return 0;
}
```
13. 见代码
```c
#include<stdio.h>
int main()
{   
    int which_word;
    scanf("%d\n", &which_word);
    switch (which_word)
    {
    case 1:
        printf("who\n");
        break;
    case 2:
        printf("what\n");
        break;
    case 3:
        printf("when\n");
        break;
    case 4:
        printf("where\n");
        break;
    case 5:
        printf("why\n");
        break;
    default:
        printf("don't know /n");
        break;
    }
    return 0;
}
```
14. 见代码
```c
#include<stdio.h>
int main()
{
    while (hungry())
    {
        eat_humberger();
    } 
    return 0;
}
```
15. 见代码
```c
#include<stdio.h>
int main()
{
    do
    {
        eat_humberger();
    } while (hungry());
    return 0;
}
```
16. 见代码
```c
#include <stdio.h>
int main() {
    int precipitating, temperature;
    scanf("%d %d", &precipitating, &temperature);

    if (precipitating) 
    {
        if (temperature < 32)
            puts("snowing");
        else
            puts("raining");
    } 
    else 
    {
        if (temperature < 60)
            puts("cold");
        else
            puts("warm");
    }
    return 0;
}
```
