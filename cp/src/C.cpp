//
// Created by lyutoev on 28.12.22.
//

#include "iostream"
#include <unistd.h>
#include <fcntl.h>
#include "../include/note.h"

int main(int argc, char const *argv[])
{
    int fdAC[2], fdCA[2], fdCB[2];
    
    fdAC[FD_OUTPUT] = std::stoi(argv[0]);
    fdAC[FD_INPUT] = std::stoi(argv[1]);

    fdCA[FD_OUTPUT] = std::stoi(argv[2]);
    fdCA[FD_INPUT] = std::stoi(argv[3]);

    fdCB[FD_OUTPUT] = std::stoi(argv[4]);
    fdCB[FD_INPUT] = std::stoi(argv[5]);

    char c, c_ok = '+';
    int size;
    while (read(fdAC[FD_OUTPUT], &size, sizeof(int)) > 0) {
        std::string str;
        std::cout << size << std::endl;
        for (int i = 0; i < size; i ++) {
            read(fdAC[FD_OUTPUT], &c, sizeof(char));
            std::cout << size << " " << i << " " << c << "\n";
            str.push_back(c);
        }
        std::cout << str << '\n';
        
        int sizeB = str.size();
        write(fdCB[FD_INPUT], &sizeB, sizeof(int));

        write(fdCA[FD_INPUT], &c_ok, sizeof(char));
    }

    close(fdAC[FD_OUTPUT]);
    close(fdAC[FD_INPUT]);
    close(fdCA[FD_OUTPUT]);
    close(fdCA[FD_INPUT]);
    close(fdCB[FD_OUTPUT]);
    close(fdCB[FD_INPUT]);

    return EXIT_SUCCESS;
}

