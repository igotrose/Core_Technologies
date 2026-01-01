#include<stdio.h>
#include<stdarg.h>

int max_list(int v1, ...)
{
    int max = 0;
    va_list var_arg;                // 创建可变参数列表
    if (v1 > 0)
    {
        max = v1;
        va_start(var_arg, v1);      // 将可变参数的第一个参数初始化
        int tmp;
        while ((tmp = va_arg(var_arg, int)) > 0)    // 访问逐个参数列表的参数
        {
            if (tmp > max)
            {
                max = tmp;
            }
            va_end(var_arg);        // 访问参数列表的最后一个参数
        }
    }
    return max;
    
}
int main()
{
    int ret = max_list(1, 2, 4, 5, 3, -1);
    printf("%d ", ret);
    return 0;
}