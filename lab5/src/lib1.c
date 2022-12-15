#include <stdio.h>
#include <stdlib.h>

float bin_pow(float x, int y)
{
    float z = 1.0;
    while (y > 0) {
        if (y % 2 != 0) {
            z *= x;
        }
        x *= x;
        y /= 2;
    }
    return z;
}

float E(int x)
{
    printf("Вычисление числа e с апроксимацией %d\n", x);
    printf("Используя (1 + 1/x) ^ x\n");
    float base = (float) 1.0 + ((float) 1 / (float) x);
    float E = bin_pow(base, x);
    return E;
}

char* translation(long x)
{
    printf("Перевод %d в двоичную систему счисления\n", x); // binary
    int flag = 0;
    if (x < 0) 
        flag = 1, x = -x;
    int longsize = 32;
    int cnt = 0;
    char *binary = (char *) malloc(longsize * sizeof(char));
    for (int i = 0; i < longsize; i++) {
		binary[i] = 's';
	}
	while(x > 0) {
        if (x % 2 == 1) {
			binary[longsize - cnt - 1] = '1';
		} else {
			binary[longsize - cnt - 1] = '0';
		}
		x = x / 2;
		cnt ++;
	}
    if (flag)
        binary[longsize - cnt - 1] = '-';

	return binary;
}
