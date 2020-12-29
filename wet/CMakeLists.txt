cmake_minimum_required(VERSION 3.15)
project(wet)

set(CMAKE_CXX_STANDARD 11)

add_executable(wet main.cpp PCQueue.cpp Semaphore.cpp)