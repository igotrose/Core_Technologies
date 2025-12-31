#include<stdio.h>

int main()
{
    char c;
    int count;

    while( ( c = getchar() ) != EOF )   
    {
        if( c == '{' )
            count++;
        else if( c == '}' )
            count--;
    
        if( count < 0 )
            break;
    }
  
    if( count < 0 || count > 0 )
        printf( "error" );
    else
        printf( "pass" );

    return 0;
}

