#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

const char* lib1 = "./liblib1.so";
const char* lib2 = "./liblib2.so";

int main(int argc, char const *argv[])
{
    int command = 0;
    int link = 0;

    void *current_lib = dlopen(lib1, RTLD_LAZY);
    printf("\nТекущая библиотека - %d\n", link);

    char* (*translation)(long x);
    float (*E)(int x);

    translation = dlsym(current_lib, "translation");
    E = dlsym(current_lib, "E");

    while (scanf("%d", &command) != EOF) {
        switch (command) {
        case 0:
            dlclose(current_lib);
            if (link == 0) {
                current_lib = dlopen(lib2, RTLD_LAZY);
            } else {
                current_lib = dlopen(lib1, RTLD_LAZY);
            }
            link = !link;
            translation = dlsym(current_lib, "translation");
            E = dlsym(current_lib, "E");
            break;
        
        case 1:
            int x;
            scanf("%d", &x);
            printf("Ответ: %f\n", E(x));
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
        printf("\nТекущая библиотека - %d\n", link);
    }
    return 0;
}
