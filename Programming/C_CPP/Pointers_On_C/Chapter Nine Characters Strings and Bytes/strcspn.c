#include<stdio.h>
#include<string.h>

void main()
{
    char string[] = "fasdfasfafasdsgadvbavd";
    int pos = strcspn( string, "lasd");
    printf("%d", pos);
}