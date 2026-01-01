// 将大写字符转化为小写字符
// 可以直接遍历，也可以直接使用tolower()函数。


#include<stdio.h>
#include<stdlib.h>
int main()
{
    char ch;
    while ( (ch = getchar()) != EOF )
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            // ch += 'a' - 'A';
            ch = tolower(ch);
        }
        putchar(ch);
    }
    return 0;
}

// #include<stdio.h>
// #include<string.h>
// #include<stdlib.h>
// #define len 20
// int main()
// {
//     char str[len];

//     for (int i = 0; i < len - 1; i++)
//     {
//         str[i] = getchar();
//         if (str[i] >= 'A'&& str[i] <= 'Z')
//         {
//             str[i] = tolower(str[i]);
//         }
//         putchar(str[i]);
//     }

//     return 0;
// }