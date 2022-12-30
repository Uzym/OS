//
// Created by lyutoev on 28.12.22.
//

#include "iostream"
#include <unistd.h>
#include <fcntl.h>
#include "../include/note.h"

int main(int argc, char const *argv[])
{
    int fdAB[2], fdCB[2];
    
    fdAB[FD_OUTPUT] = std::stoi(argv[0]);
    fdAB[FD_INPUT] = std::stoi(argv[1]);

    fdCB[FD_OUTPUT] = std::stoi(argv[2]);
    fdCB[FD_INPUT] = std::stoi(argv[3]);

    int sizeA, sizeB;
    while (read(fdAB[FD_OUTPUT], &sizeA, sizeof(int)) > 0) {
        read(fdCB[FD_OUTPUT], &sizeB, sizeof(int));
        std::cout << "A: " << sizeA << " B: " << sizeB << "\n";
    }

    close(fdAB[FD_OUTPUT]);
    close(fdAB[FD_INPUT]);
    close(fdCB[FD_OUTPUT]);
    close(fdCB[FD_INPUT]);

    return EXIT_SUCCESS;
}


