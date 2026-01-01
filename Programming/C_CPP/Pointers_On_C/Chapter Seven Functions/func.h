#include<stdio.h>
#include<stdarg.h>

int even_parity(int value, int n_bits);

int *find_int(int key, int array[], int array_len);

void swap(int *x, int *y);

void clear_array(int array[], int n_elements);

#define NAME_LENGTH 30
#define ADDR_LENGTH 100
#define PHONE_LENGTH 11

#define MAX_ADDRESSES 1000

char const *lookup_phone(char const *name);
char const *lookup_address(char const *name);

void binary_to_ascii(unsigned int value);

long factorial1(int n);
long factorial2(int n);

long fibonacci1(int n);
long fibonacci2(int n);

float average_lower(int n_value, int v1, int v2, int v3, int v4, int v5);