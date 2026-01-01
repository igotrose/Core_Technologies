// 查找子串最右一次出现的位置
#include<string.h>

char* strrstr(const char* s1, const char* s2)
{
    register char* last;
    register char* current;
    last = NULL;
    if (*s2 != '\0')
    {
        current = strstr(s1, s2);
        while (current != NULL)
        {
            last = current;
            current = strstr(last + 1, s2);
        }
    }
}