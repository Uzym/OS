cmake_minimum_required(VERSION 3.23)
project(kurs)

set(CMAKE_CXX_STANDARD 14)

add_library(
    yaml_parser
)

add_executable(test_job src/test_job.cpp)
add_executable(job src/job.cpp src/job.h)
add_executable(kurs src/manage_dag.cpp)
