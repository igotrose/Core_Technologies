#include<stdio.h>
#include<math.h>
#include<stdbool.h>

// 题目描述的是埃氏筛，那么在此就将素数的求法进行求解汇总

#define N 1000

int primes[N];
int count;
int st[N + 1];

// 暴力筛法
int easy_prime(int n)
{
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
        {
            return 0;
        }   
    }
    return 1;
}

// 埃式筛法 —— 在朴素筛法的基础上进行优化
void eratosthenes_sieve_method() 
{
    for (int i = 2; i < N; ++i) 
    {
        if (!st[i]) 
        {
            primes[count++] = i;
            for (int j = i + i; j < N; j += i)
            {
                st[j] = 1;
            }
        }
    }
}

// 欧拉筛法 —— 线性筛法 —— 保证合数只会被最小质因子筛去，并保证每个合数都会被筛去
void euler_sieve_method()
{
    for (int i = 2; i < N; i++)
    {
        if (!st[i])
        {
            primes[count++] = i;
        }
        for (int j = 0; j < count && i * primes[j] < N; j++)
        {
            st[i * primes[j]] = 1;
            if (i % primes[j] == 0)
            {
                break;
            }
        }
    }
}

int main()
{
    int n = 1000;
    // 暴力筛法
    for (int i = 2; i < n; i++)
    {
        if (easy_prime(i))
        {
            printf("%d ", i);
        }    
    }
    puts("\n");
    
    // 埃式筛法
    eratosthenes_sieve_method();
    for (int i = 0; i < count; i++)
    {
        printf("%d ", primes[i]);
    }
    printf("\n");

    // 欧拉筛法
    euler_sieve_method();
    for (int i = 0; i < count; i++)
    {
        printf("%d ", primes[i]);
    }
    return 0;
}