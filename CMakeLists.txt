cmake_minimum_required (VERSION 3.5)
project (snake)

file(GLOB SOURCES "src/*.c" "src/*.h")

add_executable (snake ${SOURCES})