// 使用牛顿迭代法求解
#include<stdio.h>
const double eps = 1e-6;        // 表示迭代的精度
int main()
{
    double n;                   
    scanf("%lf\n", &n);         // 读取数值
    if (n < 0)
    {
        printf("input error\n");// 数值判断
    }
    double new = 1, last;       // 当前平方根迭代的近似值和上次迭代的值
    do
    {
        last = new;
        new = (last + n/last)/2;// 给出的公式
        printf("%.4f\n", new);
    } while (last - new <= eps);
    return 0;
}