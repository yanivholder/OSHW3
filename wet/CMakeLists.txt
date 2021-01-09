cmake_minimum_required(VERSION 3.15)
project(wet)

set(CMAKE_CXX_STANDARD 11)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

#add_executable(part1 ./Code_Skeleton/main_part1.cpp ./Code_Skeleton/Semaphore.cpp)

add_executable(part2 ./Code_Skeleton/main.cpp ./Code_Skeleton/Semaphore.cpp
        ./Code_Skeleton/Game.cpp ./Code_Skeleton/utils.cpp ./Code_Skeleton/Thread.hpp)

#add_executable(tests ./my_main.cpp)
