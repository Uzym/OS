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
#include <string.h>

int main(int argc, char const *argv[])
{
    char* map = (char*) mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if (map == MAP_FAILED) {
        std::cerr << "mmap failed\n";
        return 1;
    }
    std::string fileName;
    std::cout << "Input file path:\n";
    std::getline(std::cin, fileName);
    int fd = open(fileName.c_str(), O_RDONLY);
    if (fd == -1) {
        std::cerr << "file error\n";
        return 1;
    }
    sem_unlink("_sem");
    sem_t *sem = sem_open("_sem", O_CREAT, 0, 1);
    int state = 0;
    while (state++ < 2) { // 1
        sem_post(sem);
    }
    while (state-- > 1) {
        sem_wait(sem);
    }
    pid_t id = fork();
    if (id == 0) {
        if (dup2(fd, 0) == -1) {
            std::cerr << "dub error\n";
            return -1;
        }
        int sum = 0, x;
        char c;
        while (1) {
            std::cin >> x;
            sum += x;
            c = getchar();
            if (c == EOF) {
                sem_post(sem);
                sem_post(sem);
                sem_post(sem);
                break;
            }
            if (c == '\n') {
                memcpy(map, &sum, sizeof(int));
                sum = 0;
                sem_wait(sem); // 0
                sem_wait(sem);
            }
        }
        munmap(map, sizeof(int));
        sem_close(sem);
        sem_destroy(sem);
    } else {
        do {
            sem_getvalue(sem, &state);
            if (state == 0) {
                int *allocated = (int *) malloc(sizeof(int));
                memcpy(allocated, map, sizeof(int));
                std::cout << *allocated << "\n";
                free(allocated);
                sem_post(sem);
                sem_post(sem);
            }
        } while (state <= 3);
        munmap(map, sizeof(int));
        sem_close(sem);
        sem_destroy(sem);
    }

    return 0;
}
