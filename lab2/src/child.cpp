#include <unistd.h>
#include <iostream>
#include "../include/note.h"

int main(int argc, char const *argv[])
{
    int fd[2];
    fd[FD_OUTPUT] = std::stoi(argv[0]);
    fd[FD_INPUT] = std::stoi(argv[1]);
    //if (dup2(fd[FD_INPUT], STDOUT) == -1) {
    //    std::cerr << "dub error\n";
    //    return -1;
    //}

    int file = std::stoi(argv[2]);
    if (dup2(file, STDIN) == -1) {
        std::cerr << "dub error\n";
        return -1;
    }

    int x, sum = 0;
    char c;
    do {
        std::cin >> x;
        sum += x;
        c = getchar();
        if (c == '\n') {
            write(fd[FD_INPUT], &c, sizeof(c));
            write(fd[FD_INPUT], &sum, sizeof(sum));
            sum = 0;
        }
    } while(c != EOF);
    write(fd[FD_INPUT], &c, sizeof(c));

    close(fd[FD_INPUT]);
    close(fd[FD_OUTPUT]);

    return 0;
}
