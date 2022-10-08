//
// Created by lyutoev on 02.10.22.
//

#ifndef LAB3_MAIN_H
#define LAB3_MAIN_H

int get_minrun(int N);

int min(int a, int b);

void sort_merge(int *arr, int l, int m, int r);

typedef struct sort_insert_arg_t {
    int *arr;
    int N;
} sort_insert_arg;

struct thread {
    pthread_t id;
    bool free;
};


#endif //LAB3_MAIN_H
