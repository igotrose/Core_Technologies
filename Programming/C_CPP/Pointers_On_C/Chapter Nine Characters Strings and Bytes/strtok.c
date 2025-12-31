#include<stdio.h>
#include<string.h>

char str[] = "A string\t of ,, tokens\nand some more tokens";
char seqs[] = " ,\t\n";
char* token;

void main()
{
    printf("%s\n\n, Token:\n", str);
    token = strtok( str, seqs );
    while (token != NULL)
    {
        printf("%s\n", token);
        token = strtok( NULL, seqs );
    }
}