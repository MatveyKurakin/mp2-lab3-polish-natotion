cmake_minimum_required(VERSION 3.24)
project(polish_notation)
include_directories( include )

add_executable(polish_notation src/main.cpp include/polish_notation.h)