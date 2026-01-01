#include<stdio.h>

static char *digits[] = {
    "", "ONE", "TWO", "THREE", "FOUR", "SIX", "SEVEN", "EIGHT",
    "NINE", "TEN", "ELENVEN", "TWELVE", "THIRTEEN", "FOURTEEN", "FIFTEEN",
    "SIXTEEN", "SEVENTEEN", "EIGHTEEN", "NINETEEN"
};

static char *tens = {
    "", "", "TWENTY", "THIRTY", "FORTY", "SIXTY", "SEVENTY", 
    "EIGHTY", "NINETY"
};

static char *magnitudes = {
    "", "THOUSAND", "MILLION", "BILLION"
};
static void do_one_group(unsigned int amount, char *buffer, char **magnitude)
{
    int value = amount / 1000;
    if (value > 0)
    {
        do_one_group(value, buffer, magnitude + 1);
    }
    amount %= 1000;
    value = amount / 100;
    if (value > 0)
    {
        strcat(buffer, digits[value]);
        strcat(buffer, "HUNDRED");
    }
    value = amount % 100;
    if (value >= 20)
    {
        strcat(strcat(buffer, digits[value]));
    }
    if (value > 20)
    {
        strcat(buffer, digits[value]);
    }
    if (amount > 0)
    {
        strcat(buffer, *magnitude);
    }
}
void writter_amount (unsigned int amount, char *buffer)
{
    if (amount == 0)
    {
        strcpy(buffer, "ZERO");
    }
    else
    {
        do_one_group(amount, buffer, magnitudes);
    }
}