//
// Created by lyutoev on 27.10.22.
//
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "../include/main.h"

using namespace std;

struct data {
    int sum;
    int st;
};

int sem_get(sem_t *sem) {
    int val;
    sem_getvalue(sem, &val);
    return val;
}

void sem_set_value(sem_t *sem, int n)
{
    while(sem_get(sem) < n) {
        sem_post(sem);
    }
    while(sem_get(sem) > n) {
        sem_wait(sem);
    }
}

int main() {
    data* map = (data*)mmap(0, sizeof(data), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if (map == MAP_FAILED) {
        cerr << "mmap failed\n";
        return 1;
    }
    sem_unlink("_sem");
    sem_t *sem = sem_open("_sem", O_CREAT, 0, 2);

    cout << "Input file path: ";
    string file_name;
    getline(cin, file_name);

    int file = open(file_name.c_str(), O_RDONLY);
    if (file == -1) {
        cerr << "file error\n";
        return 1;
    }
    sem_set_value(sem, SEM_WAIT);
    map->st = 0;

    pid_t fork_id = fork();
    if (fork_id == -1) {
        cerr << "fork error\n";
        return 1;
    } else if (fork_id == 0) {

        if (dup2(file, 0) == -1) {
            std::cerr << "dub error\n";
            return -1;
        }
        char c;
        int x, sum = 0;
        do {
            while (map->st != 0);
            if (sem_get(sem) == SEM_GET) {
                continue;
            }
            std::cin >> x;
            sum += x;
            c = getchar();
            if (c == '\n') {
                map->sum = sum;
                map->st = 1;
                if (sem_get(sem) == SEM_WAIT) {
                    sem_set_value(sem, SEM_GET);
                }
                sum = 0;
            }
        } while(c != EOF);
        sem_set_value(sem, SEM_EXIT);
        map->st = 2;
        close(file);

    } else {
        while (sem_get(sem) != SEM_EXIT && map->st != 2) {
            while (map->st == 0);
            if (sem_get(sem) == SEM_WAIT) {
                continue;
            } else if (sem_get(sem) == SEM_GET) {
                cout << map->sum << "\n";
                map->st = 0;
                sem_set_value(sem, SEM_WAIT);
            }
        }
    }
    munmap(map, sizeof(data));
    sem_close(sem);
    sem_destroy(sem);

    return 0;

}
