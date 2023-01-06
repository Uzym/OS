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

int main(int argc, char const *argv[])
{
    int fdAB[2], fdCB[2];
    
    fdAB[FD_OUTPUT] = std::stoi(argv[0]);
    fdAB[FD_INPUT] = std::stoi(argv[1]);

    fdCB[FD_OUTPUT] = std::stoi(argv[2]);
    fdCB[FD_INPUT] = std::stoi(argv[3]);

    sem_t* semA = sem_open("semA", O_CREAT, 0777, 1);
    sem_t* semB = sem_open("semB", O_CREAT, 0777, 0);
    sem_t* semC = sem_open("semC", O_CREAT, 0777, 0);

    size_t sizeA, sizeC;
    while (sem_get(semB) != END) {
        sem_wait(semB);
        if (sem_get(semB) == END) {
            break;
        }
        read(fdAB[FD_OUTPUT], &sizeA, sizeof(sizeA));
        std::cout << "A: " << sizeA << "\n";
        
        sem_post(semC);
        sem_wait(semB);
        if (sem_get(semB) == END) {
            break;
        }

        read(fdCB[FD_OUTPUT], &sizeC, sizeof(sizeC));
        std::cout << "C: " << sizeC << "\n";

        sem_post(semA);
        if (sem_get(semB) == END) {
            break;
        }
    }

    sem_close(semA);
    sem_close(semB);
    sem_close(semC);

    close(fdAB[FD_OUTPUT]);
    close(fdAB[FD_INPUT]);
    close(fdCB[FD_OUTPUT]);
    close(fdCB[FD_INPUT]);

    return EXIT_SUCCESS;
}


