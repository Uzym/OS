#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "../include/note.h"

int main() {

    std::string file_name;
    if (!getline(std::cin, file_name)) {
        std::cerr << "input error\n";
        return -1;
    }

    int file = open(file_name.c_str(), O_RDONLY);
    if (file == -1) {
        std::cerr << "file error\n";
        return -1;
    }

    int fd[2];
    if (pipe(fd) == -1) {
        std::cerr << "pipe error\n";
        return -1;
    }

    pid_t fork_id = fork();
    if (fork_id == -1) {
        std::cerr << "fork error\n";
        return -1;
    } else if (fork_id == 0) {
        execlp("./child",
               std::to_string(fd[FD_OUTPUT]).c_str(),
               std::to_string(fd[FD_INPUT]).c_str(),
               std::to_string(file).c_str(),
               NULL
        );
    } else {
        int sum;
        char buf;
        while (read(fd[FD_OUTPUT], &buf, sizeof(buf)) > 0 && buf != EOF) {
            read(fd[FD_OUTPUT], &sum, sizeof(sum));
            std::cout << sum << std::endl;
        }
        /*
        if (dup2(STDOUT, fd[FD_OUTPUT]) == -1) {
            std::cerr << "dub error\n";
            return -1;
        }
        */
    }

    close(fd[FD_INPUT]);
    close(fd[FD_OUTPUT]);
    close(file);

    return 0;
}
