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
    int fdAC[2], fdCA[2], fdCB[2];
    
    fdAC[FD_OUTPUT] = std::stoi(argv[0]);
    fdAC[FD_INPUT] = std::stoi(argv[1]);

    fdCB[FD_OUTPUT] = std::stoi(argv[2]);
    fdCB[FD_INPUT] = std::stoi(argv[3]);

    sem_t* semA = sem_open("semA", O_CREAT, 0777, 1);
    sem_t* semB = sem_open("semB", O_CREAT, 0777, 0);
    sem_t* semC = sem_open("semC", O_CREAT, 0777, 0);

    char c;
    int size;
    while (sem_get(semC) != END) {
        sem_wait(semC);
        if (sem_get(semC) == END) {
            break;
        }
        read(fdAC[FD_OUTPUT], &size, sizeof(int));
        std::string str;
        for (int i = 0; i < size; i ++) {
            read(fdAC[FD_OUTPUT], &c, sizeof(char));
            str.push_back(c);
        }
        std::cout << str << '\n';
        
        std::size_t sizeB = str.size();
        write(fdCB[FD_INPUT], &sizeB, sizeof(std::size_t)); 

        sem_post(semB);
    }
    
    sem_close(semA);
    sem_close(semB);
    sem_close(semC);

    close(fdAC[FD_OUTPUT]);
    close(fdAC[FD_INPUT]);
    close(fdCB[FD_OUTPUT]);
    close(fdCB[FD_INPUT]);

    return EXIT_SUCCESS;
}

