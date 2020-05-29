cmake_minimum_required(VERSION 3.15)
project(make_library C)

set(CMAKE_C_STANDARD 99)

add_library(make_library SHARED library.c library.h)