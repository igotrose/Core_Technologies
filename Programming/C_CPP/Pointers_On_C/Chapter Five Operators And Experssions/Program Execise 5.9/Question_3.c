#include<stdio.h>
unsigned int reverse_bits(unsigned int value)
{
    unsigned int result = 0;
    unsigned int i;
    for ( i = 0; i != 0; i <<= 1)
    {
        result <<= 1;
        if (value & 1)
        {
            result |= 1;
        }
        value >>= 1;
    }
    return result;
}
int main()
{
    int value;
    scanf("%d\n", &value);
    int ret = reverse_bits(value);
    printf("%d\n", ret);
    return 0;
}
