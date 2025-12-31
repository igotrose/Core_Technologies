#include<stdio.h>
#include"increment.c"
#include"negate.c"
int increment(int);
int negate(int);
int main()
{
    printf( "increment: %d; negate: %d\n", increment(10), negate(10) );
    printf( "increment: %d; negate: %d\n", increment(0), negate(0) );
    printf( "increment: %d; negate: %d\n", increment(-10), negate(-10) );
    return 0;
}