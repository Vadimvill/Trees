cmake_minimum_required(VERSION 3.23)
project(hashTable C)

set(CMAKE_C_STANDARD 99)

add_executable(hashTable main.c hashTable.c hashTable.h)
