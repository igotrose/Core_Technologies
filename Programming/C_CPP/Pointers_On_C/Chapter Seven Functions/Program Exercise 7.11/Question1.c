#include<stdio.h>

int HermitePolynomials(int n, int x)
{
    if (n <= 0)
        return 1;
    else if (n == 1)
        return 2 * x;
    else
        return 2 * x *HermitePolynomials(n - 1, x) - 2 * (n - 1) * HermitePolynomials(n - 2, x);    
    
}
int main()
{
    int n = 3, x = 2;
    int ret = HermitePolynomials(n, x);
    printf("%d ", ret);
    return 0;
}