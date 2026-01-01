#include<stdio.h>

char *find_char( char const *source, char const *chars)
{
    // 创建临时指针变量，遍历字符串
    // 由于chars指针是常量字符指针，所以GCC希望也使用一个常量字符指针
    const char *tmp;
    if (source != NULL && chars != NULL)
    {
         while ( *source != '\0')
        {
            // 把tmp指针指向chars
            tmp = chars;
            while ( *tmp != '\0')
            {
                if (*source == *tmp)
                {
                    return source;
                }
                ++tmp;
            }
            ++source;
        }
    }
    return NULL;
}

int main()
{   
    char source[] = "ABCDEF";
    char chars[] = "AM";
    // 返回的是一个指针，需要用指针变量接收
    char *find = find_char(source, chars);
    printf("%p\n%c", find, *find);
    return 0;
}