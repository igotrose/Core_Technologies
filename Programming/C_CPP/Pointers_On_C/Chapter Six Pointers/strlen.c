#include<stdlib.h>
/*
string 字符串的地址
*string 字符串的值
*/
size_t strlen(char *string)
{
    int length = 0;
    while (*string++ != '\0')
    {
        length += 1;
    }
    return length;
}