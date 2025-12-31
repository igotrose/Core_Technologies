#include<stdio.h>
#include<float.h>

float single_tax(float income)
{
    float tax_rate[] = {0.15, 0.28, 0.31, 0.36, 0.396};
    float tax_base[] = {0, 3502.5, 12798.5, 31832.5, 81710.5};
    float tax_more[] = {0, 23350, 56550, 117950, 256500, DBL_MAX};
    int i = 0;
    for ( i = 1; income >= tax_more[i]; i++)
    ;
    i--;
    return tax_base[i] + (income - tax_more[i])*tax_rate[i];  
}
