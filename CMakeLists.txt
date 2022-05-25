cmake_minimum_required(VERSION 3.16)
project(KSP C)

set(CMAKE_C_STANDARD 99)

add_compile_options(-g -Wall -pedantic)

add_executable(KSP njvm.c)
