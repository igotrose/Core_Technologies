// 第三章 数据
#include<stdio.h>
int main()
{   // 3.1 基本数据类型——整形、浮点型、指针和聚合类型（如数组和结构等）
    /*
    整型家族：
    包括字符、短整型、整形和长整型；
    都分为有符号和无符号；
        整型字面值
            与变量相似，区别于初始化后不能更改
        枚举类型

    */ 
    // char类型变量的目的是为了让他们容纳字符型值
    char d = 'a';   //单一字符
    char s[] = "abc";   //字符串需要用字符数组储存
    int a = 1;
    short b = 2;
    long c = 3;
    printf("%c\n", d);  
    printf("%zu\n", sizeof(s)); //%zu是格式说明符
    printf("%d %d %d \n%", a, b, c);
    printf("%zu %zu %zu \n", sizeof(a), sizeof(b), sizeof(c));
    // 声明Jar_Type类型
    // 方法一：enum Jar_Type{ CUP, PINK, QUART};      
    // 方法二：enum Jar_Type milk_jug, gas_can;
    // 方法三：enum {CUP, PINK, QUART, milk_jug, gas_can}；

    /*
    浮点类型
    浮点数字面值在缺省的情况下都是double类型
    */
    float pi = 3.1415926;
    double r = 0.0000003;
    printf("%f %0.2e\n", pi, r);

    /*
    指针
    指针可以有效地实现注入tree和list这类高级数据结构
    变量得知存储于计算机的内存内，每一个变量都占据一个特定的位置。
    每个内存位置都是有地址唯一确定并引用。指针只是地址的另一个名字
    指针变量就是一个值为另一个内存地址的变量
    */


    // 基本声明
    int j = 15;     // 初始化
    int values[20]; // 声明简单数组
    int *af;         // 声明指针
    char *message;
    message = "Hello World";

    // 隐式声明
    //typedef （起别名）
    // char *ptr_to_char;                  // 变量ptr_to_char是一个指向字符的指针
    // typedef char *ptr_to_char;          // 把这个声明用ptr_to_char替换
    // ptr_to_char p;                      // p是指向字符的指针

    // 常量
    /* 
    作用域
    文件作用域、函数作用域、代码块作用域、原型作用域
    */

    /*
    链接属性（不解）
        外部链接、内部链接、无连接
    */

    /*
    存储类型
    是指存储变量值的内存类型
    */

    /*
    初始化
    */

    /*
    static关键字
    */

    return 0;
}