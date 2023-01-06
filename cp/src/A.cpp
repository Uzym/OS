//
// Created by lyutoev on 28.12.22.
//
#include "iostream"
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include "../include/note.h"

int sem_get(sem_t *sem)
{
    int state;
    sem_getvalue(sem, &state);
    return state;
}

int main()
{

    int fdAC[2];
    if (pipe(fdAC) == -1) {
        std::cerr << "pipe error\n";
        return EXIT_FAILURE;
    }

    int fdAB[2];
    if (pipe(fdAB) == -1) {
        std::cerr << "pipe error\n";
        return EXIT_FAILURE;
    }

    int fdCB[2];
    if (pipe(fdCB) == -1) {
        std::cerr << "pipe error\n";
        return EXIT_FAILURE;
    }

    sem_unlink("semA");
    sem_t* semA = sem_open("semA", O_CREAT, 0777, 1);
    if (semA == SEM_FAILED) {
        std::cerr << "semA error\n";
        return EXIT_FAILURE;
    }
    while (sem_get(semA) > START) {
        sem_wait(semA);
    }

    sem_unlink("semB");
    sem_t* semB = sem_open("semB", O_CREAT, 0777, 0);
    if (semB == SEM_FAILED) {
        std::cerr << "semB error\n";
        return EXIT_FAILURE;
    }
    while (sem_get(semB) > START) {
        sem_wait(semB);
    }
    
    sem_unlink("semC");
    sem_t* semC = sem_open("semC", O_CREAT, 0777, 0);
    if (semC == SEM_FAILED) {
        std::cerr << "semC error\n";
        return EXIT_FAILURE;
    }
    while (sem_get(semC) > START) {
        sem_wait(semC);
    }

    pid_t B_pid = fork();
    if (B_pid == -1) {
        std::cerr << "fork error\n";
        return EXIT_FAILURE;
    } else if (B_pid == 0) {
        execlp(
            "./B",
            std::to_string(fdAB[FD_OUTPUT]).c_str(),
            std::to_string(fdAB[FD_INPUT]).c_str(),
            std::to_string(fdCB[FD_OUTPUT]).c_str(),
            std::to_string(fdCB[FD_INPUT]).c_str(),
            NULL
        );
    }

    pid_t C_pid = fork();
    if (C_pid == -1) {
        std::cerr << "fork error\n";
        return EXIT_FAILURE;
    } else if (C_pid == 0) {
        execlp(
            "./C",
            std::to_string(fdAC[FD_OUTPUT]).c_str(),
            std::to_string(fdAC[FD_INPUT]).c_str(),
            std::to_string(fdCB[FD_OUTPUT]).c_str(),
            std::to_string(fdCB[FD_INPUT]).c_str(),
            NULL
        );
    }
    
    std::string str;
    size_t size;
    while (getline(std::cin, str)) {
        size = str.size();
        write(
            fdAB[FD_INPUT], 
            &size, 
            sizeof(size)
        );
        write(
            fdAC[FD_INPUT],
            &size, 
            sizeof(int)
        );
        for (int i = 0; i < size; i ++) {
            char c = str[i];
            write(
                fdAC[FD_INPUT], 
                &c, 
                sizeof(char)
            );
        }
        sem_post(semB);
        sem_wait(semA);
    }
    
    while (sem_get(semC) < END) {
        sem_post(semC);
    }
    while (sem_get(semB) < END) {
        sem_post(semB);
    }

    sem_close(semA);
    sem_close(semB);
    sem_close(semC);

    sem_destroy(semA);
    sem_destroy(semB);
    sem_destroy(semC);

    close(fdAB[FD_OUTPUT]);
    close(fdAB[FD_INPUT]);
    close(fdAC[FD_OUTPUT]);
    close(fdAC[FD_INPUT]);
    close(fdCB[FD_OUTPUT]);
    close(fdCB[FD_INPUT]);

    return EXIT_SUCCESS;
}
