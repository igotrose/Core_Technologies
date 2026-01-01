#include<stdio.h>

int gcd(int M, int N)
{
    int R;
    if (M <= 0 || N <= 0)
        return 0;
    else if (M % N == 0)
        return N;
    else
        R = M % N;
        return gcd(N, R);
}

int main()
{
    int M = 24, N = 18;
    int ret = gcd(M, N);    
    printf("%d", ret);
    return 0;
}