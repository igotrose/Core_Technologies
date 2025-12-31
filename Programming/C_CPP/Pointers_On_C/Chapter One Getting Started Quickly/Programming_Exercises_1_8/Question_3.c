#include<stdio.h>
int main()
{
	int ch;
	char checksum = -1;
	while ((ch = getchar()) != EOF)
	{
		checksum += ch;
		if (ch == '\n')
		{
			printf(" %d\n", checksum);
			checksum = -1;
		}
		putchar(ch);
	}
    return 0;
}
