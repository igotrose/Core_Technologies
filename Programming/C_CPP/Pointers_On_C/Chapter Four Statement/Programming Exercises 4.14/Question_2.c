// 求解1~100的质数
#include<stdio.h>
int main()
{
    int prime;
    int i;
    for (prime = 2; prime < 100; prime++)
    {
        for(i = 2; i <= prime; i++)
        {
            if(prime % i == 0 && i != prime)
            {
                break;
            }
            else if (prime == i)
            {
                printf("%d\n", prime); 
            }
        }
    }
    return 0;
}