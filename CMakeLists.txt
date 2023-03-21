cmake_minimum_required(VERSION 3.1)
project(snake)

file(GLOB SOURCES
    "src/*.c"
    "src/*.h"
)

add_executable(snake ${SOURCES})