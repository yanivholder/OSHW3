cmake_minimum_required(VERSION 3.15)
project(wet)

set(CMAKE_CXX_STANDARD 11)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

#add_executable(part1 ./Code_Skeleton/Part1/main_part1.cpp ./Code_Skeleton/Part1/Semaphore.cpp)

add_executable(part2 ./Code_Skeleton/Part2/main.cpp ./Code_Skeleton/Part1/Semaphore.cpp
        ./Code_Skeleton/Part2/Game.cpp ./Code_Skeleton/Part2/utils.cpp ./Code_Skeleton/Part2/Thread.hpp)

#add_executable(tests ./my_main.cpp)
