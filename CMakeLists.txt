cmake_minimum_required(VERSION 3.22)
project(test)

set(CMAKE_CXX_STANDARD 17)

add_executable(test main.cpp src/ashnet.cpp include/ashnet.h include/ashnet.tpp)
