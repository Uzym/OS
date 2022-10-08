//
// Created by lyutoev on 02.10.22.
//

#include <iostream>
#include <pthread.h>
#include "../include/main.h"
#include <unistd.h>
#include <ctime>

void* sort_insert(void* arg) // сортировка вставкой
{
    auto *args = (sort_insert_arg*) arg;
    int new_elem, location;
    for (int i = 1; i < args->N; i ++) {
        new_elem = args->arr[i];
        location = i - 1;
        while (location >= 0 && args->arr[location] > new_elem) {
            args->arr[location + 1] = args->arr[location];
            location = location - 1;
        }
        args->arr[location + 1] = new_elem;
    }
    free(arg);
    return NULL;
}

/*
50
9 1 0 8 7 9 7 6 9 2 0 9 8 5 4 3 3 2 2 4 6 5 5 4 5 9 0 8 6 7 9 7 7 6 9 8 0 9 8 9 5 4 3 0 2 2 6 5 5 4
*/
int main(int argc, char* argv[]) {
    if (argc != 2) {
        return 1;
    }
    int threadNum = atoi(argv[1]);
    thread threads[threadNum];
    for (int i = 0; i < threadNum; i++) {
        threads[i].id = -1;
        threads[i].free = true;
    }

    // надо изменить ввод, например считывать с файла
    int N;
    std::cin >> N;
    if (N < 1) {
        return 1;
    }

    int array[N];
    for (int i = 0; i < N; i++) {
        std::cin >> array[i];
    }
    int minrun = get_minrun(N); // минимальный размер последовательности
    std::cout << "minrun: " << minrun << '\n';

    int threadItr = 0;

    sort_insert_arg *args;
    for (int i = 0; i < N; i += minrun) {
        args = (sort_insert_arg *) malloc(sizeof* args);
        args->arr = &array[i];
        if (minrun + i - 1 < N - 1) {
            args->N = minrun;
        } else {
            args->N = N - i;
        }
        if (threadItr == threadNum) {
            threadItr = 0;
            for (int k = 0; k < threadNum; k++) {
                pthread_join(threads[k].id, NULL);
            }
        }
        pthread_create(&threads[threadItr].id, NULL, sort_insert, args);
        threadItr ++;
    }

    for (int i = 0; i < threadItr; i++) {
        pthread_join(threads[i].id, NULL);
    }

    for (auto e: array)
        std::cout << e << ' ';
    std::cout << '\n';


    for (int size = minrun; size < N; size = 2*size) {
        for (int l = 0; l < N; l += 2*size) {
            int mid = l + size - 1;
            int right = min(l + 2*size - 1, N - 1);
            if (mid < right) {
                sort_merge(array, l, mid, right);
            }
        }

    }
    //for (auto e : array)
    //    std::cout << e << ' ';
    //std::cout << '\n';

}

int min(int a, int b) {
    if (a > b)
        return b;
    return a;
}

int get_minrun(int N) // расчитываем минимальный размер последовательности
{
    int R = 0;
    while(N >= 64) {
        R |= N & 1;
        N >>= 1;
    }
    return N + R;
}

void sort_merge(int *arr, int l, int m, int r)
{
    int len1 = m - l + 1, len2 = r - m;
    int left[len1], right[len2];
    for (int i = 0; i < len1; i++)
        left[i] = arr[l + i];
    for (int i = 0; i < len2; i++)
        right[i] = arr[m + 1 + i];

    int i = 0;
    int j = 0;
    int k = l;

    while (i < len1 && j < len2) {
        if (left[i] <= right[j]) {
            arr[k] = left[i];
            i++;
        } else {
            arr[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < len1) {
        arr[k] = left[i];
        k++;
        i++;
    }

    while (j < len2) {
        arr[k] = right[j];
        k++;
        j++;
    }
}
