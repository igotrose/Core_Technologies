#include<stdio.h>
#include<string.h>

int main()
{
    const char src[] = "get no effort";
    char* dst;
    memcpy(dst, src, strlen(src));
    printf("%s", dst);
    return 0;
}