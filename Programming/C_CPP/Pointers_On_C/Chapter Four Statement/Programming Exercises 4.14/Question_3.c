// 编写程序，判断三角形
#include<stdio.h>
int main()
{
    int a, b, c;
    scanf("%d %d %d\n",&a, &b, &c);
    if (a + b > c || a + c > b || b + c > a)
    {
        printf("These three sides can form a triangle\n");
        if (a == b && b == c && c == a)
        {
            printf("This is an equilateral triangle\n");
        }
        else if (a == b || a == c || c == b)
        {
            printf("This is an isosceles triangle");
        }
        else
        {
            printf("This is an scalene triangle\n");
        }
    }
    else
    {
        printf("These three sides cannot form a triangle\n");
    }
    
    return 0;
}