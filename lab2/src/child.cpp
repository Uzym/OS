#include <unistd.h>
#include <string>
#include <iostream>
#include <algorithm>

int main(int argc, char const *argv[])
{
    int fd1[2], fd2[2], fd3[2];
    fd1[0] = std::stoi(argv[0]);
    fd1[1] = std::stoi(argv[1]);
    fd2[0] = std::stoi(argv[2]);
    fd2[1] = std::stoi(argv[3]);
    fd3[0] = std::stoi(argv[4]);
    fd3[1] = std::stoi(argv[5]);

    pid_t id = fork();
    if(id == -1) {
        std::cerr << "fork error\n";
        return -1;
    } else if (id == 0) {
        while(true) {
            fflush(stdout);
            size_t len;
            read(fd2[0], &len, sizeof(len));
            char *buf = new char[len];
            read(fd2[0], buf, len);
            
            std::string s = std::string(buf);
            delete[] buf;

            std::replace(s.begin(), s.end(), ' ', '_');

            write(fd3[1], &len, sizeof(len));
            write(fd3[1], s.c_str(), len);
        }
    } else {
        while(true) {
            fflush(stdout);
            size_t len;
            read(fd1[0], &len, sizeof(len));
            char *buf = new char[len];
            read(fd1[0], buf, len);
            
            std::string s = std::string(buf);
            delete[] buf;

            transform(s.begin(), s.end(), s.begin(), tolower);

            write(fd2[1], &len, sizeof(len));
            write(fd2[1], s.c_str(), len);
        }
    }

    close(fd1[0]);
    close(fd1[1]);
    close(fd2[0]);
    close(fd2[1]);

    return 0;
}
