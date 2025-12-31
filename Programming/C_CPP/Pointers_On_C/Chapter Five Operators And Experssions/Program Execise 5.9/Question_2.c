// 凯撒密码

#include<stdio.h>
int encrypt(char ch , int base)
{
    // 肥肠零活！！
    ch -= base;
    ch += 13;
    ch %= 26;
    return ch + base;
}
int main()
{
    char ch;
    while ((ch = getchar()) != EOF)
    {
        if (ch >= 'A' && ch <= 'Z')
        {
            encrypt(ch, 'A');
        }
        else if (ch >= 'a' && ch <= 'z')
        {        
            encrypt(ch, 'a');
        }
        else
        {
            putchar(ch);
        }  
    }
    return 0;
}