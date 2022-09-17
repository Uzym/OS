# Лабораторная работа №2 по курсу "Операционные системы"
Студент группы: M80-207Б-21, Лютоев Илья Александрович\
Контакты: lyutoev2003ia@gmail.com\
Работа выполнена: 16.09.2022\
Преподаватель: Миронов Евгений Сергеевич

## Задание
Составить и отладить программу на языке Си, осуществляющую работу с процессами и взаимодействие между ними в одной из двух операционных систем. В результате работы
программа (основной процесс) должен создать для решение задачи один или несколько
дочерних процессов. Взаимодействие между процессами осуществляется через системные сигналы/события и/или каналы (pipe).
Необходимо обрабатывать системные ошибки, которые могут возникнуть в результате работы.

### Группа вариантов №3
Родительский процесс создает два дочерних процесса. Перенаправление стандартных потоков
ввода-вывода показано на картинке выше. Child1 и Child2 можно «соединить» между собой
дополнительным каналом. Родительский и дочерний процесс должны быть представлены
разными программами.
Родительский процесс принимает от пользователя строки произвольной длины и пересылает их в
pipe1. Процесс child1 и child2 производят работу над строками. Child2 пересылает результат своей
работы родительскому процессу. Родительский процесс полученный результат выводит в
стандартный поток вывода.

### Вариант 13
Child1 переводит строки в нижний регистр. Child2 превращает все пробельные символы в символ «\_».

### Методы и алгоритмы решения
Лабораторная работа выполнена на операционной системы Linux на языке C++.\
Для соединения `child1` и `child2` создадим новый pipe.\
Также создадим дочерний процесс `child1` с помощью `fork()` и `exec()`.\
С помощью `pipe1` мы будем передавать в него в процессе `parent` введённые пользователем строки. После чего переведём все буквы в нижний регистр и запишем получившиюся строку в `pipe2`.\
Также в `child` мы тоже создаём дочерний процесс, который будет "слушать" `pipe2` и заменять все пробелы на нижние подчеркивания, после чего передавать результат в `pipe3`, откуда его уже будет считывать `parent` и выдавать пользователю.\
Также для удобства был создан Makefile, позволяющий быстро компилировать и запускать программу.\

## Исходный код
### parent.cpp
```cpp
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
```
### child.cpp
```cpp
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

```
### Makefile
```
all: parent.cpp child.cpp
	g++ -o parent parent.cpp
	g++ -o child child.cpp
	./parent
```

## Выполнение программы

```
A B C D a b c d
a_b_c_d_a_b_c_d
a
a
        a       
________a_______
AAAAAAAAAAAAAAAAA
aaaaaaaaaaaaaaaaa
```

## Демонстрация системных вызовов с помощью strace

```
execve("./parent", ["./parent"], 0x7ffe56b310e0 /* 55 vars */) = 0
brk(NULL)                               = 0x19d9000
arch_prctl(0x3001 /* ARCH_??? */, 0x7ffd73462de0) = -1 EINVAL (Недопустимый аргумент)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f59a6a43000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (Нет такого файла или каталога)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
newfstatat(3, "", {st_mode=S_IFREG|0644, st_size=94507, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 94507, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f59a6a2b000
close(3)                                = 0
openat(AT_FDCWD, "/lib64/libstdc++.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0\315\212\206\262\230\233\36O\224\0H\34\36\241\250\v"..., 36, 800) = 36
newfstatat(3, "", {st_mode=S_IFREG|0755, st_size=2360040, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 2308096, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f59a67f7000
mprotect(0x7f59a6891000, 1609728, PROT_NONE) = 0
mmap(0x7f59a6891000, 1118208, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x9a000) = 0x7f59a6891000
mmap(0x7f59a69a2000, 487424, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1ab000) = 0x7f59a69a2000
mmap(0x7f59a6a1a000, 57344, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x222000) = 0x7f59a6a1a000
mmap(0x7f59a6a28000, 10240, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f59a6a28000
close(3)                                = 0
openat(AT_FDCWD, "/lib64/libm.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0755, st_size=919824, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 905480, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f59a6719000
mmap(0x7f59a6727000, 466944, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xe000) = 0x7f59a6727000
mmap(0x7f59a6799000, 376832, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x80000) = 0x7f59a6799000
mmap(0x7f59a67f5000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xdb000) = 0x7f59a67f5000
close(3)                                = 0
openat(AT_FDCWD, "/lib64/libgcc_s.so.1", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\0\0\0\0\0\0\0\0"..., 832) = 832
newfstatat(3, "", {st_mode=S_IFREG|0755, st_size=128512, ...}, AT_EMPTY_PATH) = 0
mmap(NULL, 127272, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f59a66f9000
mmap(0x7f59a66fc000, 94208, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x3000) = 0x7f59a66fc000
mmap(0x7f59a6713000, 16384, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1a000) = 0x7f59a6713000
mmap(0x7f59a6717000, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1d000) = 0x7f59a6717000
mmap(0x7f59a6718000, 296, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f59a6718000
close(3)                                = 0
openat(AT_FDCWD, "/lib64/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20\227\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
pread64(3, "\4\0\0\0@\0\0\0\5\0\0\0GNU\0\2\0\0\300\4\0\0\0\3\0\0\0\0\0\0\0"..., 80, 848) = 80
pread64(3, "\4\0\0\0\24\0\0\0\3\0\0\0GNU\0o\\\345\24\251\347\365\36\2G\245'\303\244\36\331"..., 68, 928) = 68
newfstatat(3, "", {st_mode=S_IFREG|0755, st_size=2375216, ...}, AT_EMPTY_PATH) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2104720, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f59a64f7000
mmap(0x7f59a651f000, 1523712, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7f59a651f000
mmap(0x7f59a6693000, 360448, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x19c000) = 0x7f59a6693000
mmap(0x7f59a66eb000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1f3000) = 0x7f59a66eb000
mmap(0x7f59a66f1000, 32144, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f59a66f1000
close(3)                                = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f59a64f5000
arch_prctl(ARCH_SET_FS, 0x7f59a64f6200) = 0
set_tid_address(0x7f59a64f64d0)         = 47396
set_robust_list(0x7f59a64f64e0, 24)     = 0
rseq(0x7f59a64f6ba0, 0x20, 0, 0x53053053) = 0
mprotect(0x7f59a66eb000, 16384, PROT_READ) = 0
mprotect(0x7f59a6717000, 4096, PROT_READ) = 0
mprotect(0x7f59a67f5000, 4096, PROT_READ) = 0
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f59a64f3000
mprotect(0x7f59a6a1a000, 53248, PROT_READ) = 0
mprotect(0x404000, 4096, PROT_READ)     = 0
mprotect(0x7f59a6a7a000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7f59a6a2b000, 94507)           = 0
getrandom("\xe7\x1e\x64\x03\xc7\xb7\x6a\x0c", 8, GRND_NONBLOCK) = 8
brk(NULL)                               = 0x19d9000
brk(0x19fa000)                          = 0x19fa000
futex(0x7f59a6a286bc, FUTEX_WAKE_PRIVATE, 2147483647) = 0
pipe2([3, 4], 0)                        = 0
pipe2([5, 6], 0)                        = 0
pipe2([7, 8], 0)                        = 0
clone(child_stack=NULL, flags=CLONE_CHILD_CLEARTID|CLONE_CHILD_SETTID|SIGCHLD, child_tidptr=0x7f59a64f64d0) = 47397
newfstatat(0, "", {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0x1), ...}, AT_EMPTY_PATH) = 0
read(0, aaaaaaa a a
"aaaaaaa a a\n", 1024)          = 12
write(4, "\f\0\0\0\0\0\0\0", 8)         = 8
write(4, "aaaaaaa a a\0", 12)           = 12
read(7, "\f\0\0\0\0\0\0\0", 8)          = 8
read(7, "aaaaaaa_a_a\0", 12)            = 12
newfstatat(1, "", {st_mode=S_IFCHR|0620, st_rdev=makedev(0x88, 0x1), ...}, AT_EMPTY_PATH) = 0
write(1, "aaaaaaa_a_a\n", 12aaaaaaa_a_a
)           = 12
read(0, bbb bfbf fb
"bbb bfbf fb\n", 1024)          = 12
write(4, "\f\0\0\0\0\0\0\0", 8)         = 8
write(4, "bbb bfbf fb\0", 12)           = 12
read(7, "\f\0\0\0\0\0\0\0", 8)          = 8
read(7, "bbb_bfbf_fb\0", 12)            = 12
write(1, "bbb_bfbf_fb\n", 12bbb_bfbf_fb
)           = 12
read(0, "", 1024)                       = 0
close(3)                                = 0
close(4)                                = 0
close(5)                                = 0
close(6)                                = 0
close(7)                                = 0
close(8)                                = 0
exit_group(0)                           = ?
+++ exited with 0 +++

```

## Вывод
В ходе выполнения работы я узнал о работе с системными вызовами, процессами и PIPEs.
