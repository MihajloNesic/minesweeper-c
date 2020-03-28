cmake_minimum_required(VERSION 3.15)
project(minesweeper_c C)

set(CMAKE_C_STANDARD 99)

add_executable(minesweeper_c src/main.c src/structs.h src/functions.h src/functions.c)