#include<stdio.h>

void matrix_multiply(int* m1, int* m2, int* r, int x, int y, int z)
{
    register int* m1p;
    register int* m2p;
    register int k;
    for (int row = 0; row < x; row++)
    {
        for (int column = 0; column < z; column++)
        {
            m1p = m1 + row * y;
            m2p = m2 + column;
            *r = 0;
            for ( k = 0; k < y; k++)
            {
                *r += *m1p * *m2p;
                m1p++;
                m2p += 2;
            }
            r++;
        }
    }
}  