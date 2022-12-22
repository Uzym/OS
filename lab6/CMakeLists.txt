cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(lab6 LANGUAGES CXX)

add_library(
    lib_tree SHARED
    ./include/tree.h
    ./src/tree.cpp
)

add_library(
    lib_zmq_manage SHARED
    ./include/zmq_manage.h
    ./src/zmq_manage.cpp 
)


add_executable(api_daemon ./src/api_daemon.cpp)
target_link_libraries(api_daemon zmq lib_tree lib_zmq_manage)
target_compile_options(api_daemon PRIVATE -fsanitize=address -g)
target_link_options(api_daemon PRIVATE -fsanitize=address -g)

add_executable(client ./src/client.cpp)
target_link_libraries(client zmq lib_tree lib_zmq_manage)
target_compile_options(client PRIVATE -fsanitize=address -g)
target_link_options(client PRIVATE -fsanitize=address -g)

add_executable(server ./src/server.cpp)
target_link_libraries(server zmq lib_zmq_manage)
target_compile_options(server PRIVATE -fsanitize=address -g)
target_link_options(server PRIVATE -fsanitize=address -g)
