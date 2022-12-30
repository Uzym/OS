//
// Created by lyutoev on 28.12.22.
//
#include "iostream"
#include <unistd.h>
#include <fcntl.h>
#include "../include/note.h"

int main()
{

    int fdAC[2];
    if (pipe(fdAC) == -1) {
        std::cerr << "pipe error\n";
        return EXIT_FAILURE;
    }

    int fdCA[2];
    if (pipe(fdCA) == -1) {
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
            std::to_string(fdCA[FD_OUTPUT]).c_str(),
            std::to_string(fdCA[FD_INPUT]).c_str(),
            std::to_string(fdCB[FD_OUTPUT]).c_str(),
            std::to_string(fdCB[FD_INPUT]).c_str(),
            NULL
        );
    }
    
    std::string str;
    char char_end = '\n', char_ok;
    while (getline(std::cin, str)) {
        size_t size = str.size();
        write(
            fdAC[FD_INPUT],
            &size, 
            sizeof(size)
        );
        write(
            fdAB[FD_INPUT], 
            &size, 
            sizeof(size)
        );
        for (int i = 0; i < size; i ++) {
            char c = str[i];
            std::cout << "A: " << c << std::endl;
            write(fdAC[FD_INPUT], &c, sizeof(char));
        }
        std::cout << "SSSSSSSSSSSSSSSSSs\n";
        read(fdCA[FD_OUTPUT], &char_ok, sizeof(char));
    }

    close(fdAB[FD_OUTPUT]);
    close(fdAB[FD_INPUT]);
    close(fdAC[FD_OUTPUT]);
    close(fdAC[FD_INPUT]);
    close(fdCA[FD_OUTPUT]);
    close(fdCA[FD_INPUT]);
    close(fdCB[FD_OUTPUT]);
    close(fdCB[FD_INPUT]);

    return EXIT_SUCCESS;
}
