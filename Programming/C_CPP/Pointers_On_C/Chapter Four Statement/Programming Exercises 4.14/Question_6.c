#include<stdio.h>
#include<string.h>
// int substr( char dst[], char src[], int start, int len)
// {
//     for (int i = start; i < len - 1; i++)
//     {
//         dst[start] = src[start];
//         if (len > strlen(src) || start == -1 || len == -1)
//         {
//             dst[start] = "";
//         }
//     }
//     return strlen(dst);
// }
int substr(char dst[], char src[], int start, int len)
{
    int src_index;
    int dst_index;

    dst_index= 0;

    if (start >= 0 && len > 0)
    {
        for (src_index = 0; src_index < start; src_index++);
        while ( len > 0 && src[src_index] != '\0')
        {
            dst[dst_index] = src[src_index];
            dst_index += 1;
            src_index += 1;
            len -= 1;
        }
    }
    dst[dst_index] = '\0';
    return dst_index;
}
int main()
{
    char src[] = "nihaofan";
    char dst[] = "nihao";
    int ret = substr(src, dst, 3, 2);   
}

