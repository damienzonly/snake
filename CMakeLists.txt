cmake_minimum_required(VERSION 3.1)
project(myproject)

file(GLOB SOURCES
    "src/*.c"
    "src/*.h"
)

add_executable(snake ${SOURCES})