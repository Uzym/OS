#include <stdio.h>
#include <stdlib.h>

long fact(int x)
{
    if (x == 0) 
        return 1;
    long n = 1;
    for (int i = 2; i <= x; i ++) {
        n *= i;
    }
    return n;
}

float machineEps(void)
{
	float e = 1.0f;
	while (1.0f + e / 2.0f > 1.0f)
		e /= 2.0f;
	return e;
}

float E(int x)
{
    printf("Вычисление числа e с апроксимацией %d\n", x);
    printf("используя сумму ряда по n от 0 до x, где элементы ряда равны: (1/(n!))\n");
    
    float maceps = machineEps();

    float E = 0;
    for (int n = 0; n <= x; n ++) {
        float sol = 1.0f / fact(n);
        float fsol = sol > 0 ? sol : (float) (-1) * sol;
        if (fsol <= maceps) {
            printf("Апроксимация сломалась из-за машинного нуля %.8f\n", maceps);
            break;
        }
        E += sol;
    }
    return E;
}

char* translation(long x)
{
    printf("Перевод %d в троичную систему счисления\n", x); // ternary
    int flag = 0;
    if (x < 0) 
        flag = 1, x = -x;
    int longsize = 32;
    int cnt = 0;
    char *ternary = (char *) malloc(longsize * sizeof(char));
    for (int i = 0; i < longsize; i++) {
		ternary[i] = 's';
	}
	while(x > 0) {
        if (x % 3 == 1) {
			ternary[longsize - cnt - 1] = '1';
		} else if (x % 3 == 2) {
			ternary[longsize - cnt - 1] = '2';
        } else {
			ternary[longsize - cnt - 1] = '0';
		}
		x = x / 3;
		cnt ++;
	}
    if (flag)
        ternary[longsize - cnt - 1] = '-';
    
	return ternary;
	
}
