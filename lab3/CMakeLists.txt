cmake_minimum_required(VERSION 3.20)
project(os_lab3)

set(CMAKE_CXX_STANDARD 14)

set ( CMAKE_C_FLAGS "-pthread -fsanitize=address")

add_executable(main ./src/main.cpp)