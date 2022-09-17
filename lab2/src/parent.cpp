#include <unistd.h>
#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
    int fd1[2];
    if (pipe(fd1) == -1) {
        std::cerr << "pipe error\n";
        return -1;
    }
    int fd2[2];
    if (pipe(fd2) == -1) {
        std::cerr << "pipe error\n";
        return -1;
    }
    int fd3[2];
    if (pipe(fd3) == -1) {
        std::cerr << "pipe error\n";
        return -1;
    }

    pid_t id = fork();
    if (id == -1) {
        std::cerr << "fork error\n";
        return -1;
    } else if (id == 0) {
        execlp("./child", 
                std::to_string(fd1[0]).c_str(), 
                std::to_string(fd1[1]).c_str(),
                std::to_string(fd2[0]).c_str(), 
                std::to_string(fd2[1]).c_str(),
                std::to_string(fd3[0]).c_str(), 
                std::to_string(fd3[1]).c_str(),
                NULL
        );
    } else {
        std::string s;
        while (getline(std::cin, s)) {
            fflush(stdout);
            size_t len = s.size() + 1;
            write(fd1[1], &len, sizeof(len));
            write(fd1[1], s.c_str(), len);

            read(fd3[0], &len, sizeof(len));
            char *buf = new char[len];
            read(fd3[0], buf, len);
            
            s = std::string(buf);
            delete[] buf;
            std::cout << s << '\n';
        }
        
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        close(fd3[0]);
        close(fd3[1]);

    }
    return 0;
}
