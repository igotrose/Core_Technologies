// 编写函数copy_n
#include<stdio.h>
void copy_n( char dst[], char src[], int n )
{
    int src_index, dst_index = 0;
    for ( dst_index = 0; dst_index < n ;dst_index++ )
    {
        dst[dst_index] = src[src_index];
        if (src[src_index] != 0)
        {
            src_index++;
        }
    }
}
int main()
{
    char a[10] = "0123456789";
    char b[6];
    copy_n(b, a, 4);
    puts(a);
    puts(b);
    return 0;
}