#include <stdio.h>
#include <stdlib.h>
#include "../include/lib.h"

int main()
{
    int com = 0;
    while (scanf("%d", &com) != EOF) {
        switch (com)
        {
        case 1:
            int a;
            scanf("%d", &a);
            printf("Ответ: %f\n", E(a));
            break;
        case 2:
            long b;
            scanf("%ld", &b);
            char* s = translation(b);
            printf("Ответ: ");
            
            for (int i = 0; i < 32; i ++) {
                if (s[i] == 's') {
                    continue;
                }
                printf("%c", s[i]);
            }
            printf("\n");
            free(s);
            break;
        default:
            printf("Неправильная команда\n");
            break;
        }
    }
}
