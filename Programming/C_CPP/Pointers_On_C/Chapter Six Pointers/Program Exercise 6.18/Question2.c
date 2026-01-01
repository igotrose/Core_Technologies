#include<stdio.h>
// 未完成
char *match(char *str, char const *match_str)
{
    while (*match_str != '\0')
    {
        if (*str++ != *match_str++)
        {
            return NULL;
        }
    }
    return str;
}
int del_substr(char *str, char const *substr)
{
    // 判断字符是否为要删除的字符串的子集
    char *next = match(str, substr);
    while (*str != '\0')
    {
        next = match(str, substr);
        if (next != NULL)
        {
            break;
        }
        str++;
    }
    if (*str == '\0')
    {
        return 0;
    }
    // 覆写
    while (*str++ = *next++)
    {
        ;
    }
    return 1;   
}
int main()
{
    char str[] = "ABCDEFG";
    char substr[] = "CDE";
    int ret = del_substr(str, substr);
    printf("%s", str);
    return 0;
}