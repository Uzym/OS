#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "../include/note.h"

int main(int argc, char const *argv[])
{
    int fd[2];
    fd[FD_OUTPUT] = std::stoi(argv[0]);
    fd[FD_INPUT] = std::stoi(argv[1]);
    if (dup2(STDOUT, fd[FD_INPUT]) == -1) {
        std::cerr << "dub error\n";
        return -1;
    }

    int file = std::stoi(argv[2]);
    if (dup2(file, STDIN) == -1) {
        std::cerr << "dub error\n";
        return -1;
    }

    int x1, x2, x3, sum;
    while (std::cin >> x1 >> x2 >> x3) {
        sum = x1 + x2 + x3;
        std::cout << sum << std::endl;
    }

    close(fd[FD_INPUT]);
    close(fd[FD_OUTPUT]);

    return 0;
}
