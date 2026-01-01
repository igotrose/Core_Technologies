#include<stdio.h>
/*
字符串转数字和数字转字符串都基于下面的表达式
0 + '0' = '0'
1 + '1' = '1'
2 + '2' = '2'
*/

void ascii_to_integer(char *string)
{
    int num = 0;
    while (*string != '\0')
    {
        if (*string >= '0'&& *string <= '9')
        {
            num = *string - '0';
            printf("%d ",num * 11);
        }
        string++;
    }
}

int main()
{
    char str[] = "adf2341dfds";
    ascii_to_integer(str);
    return 0;
}