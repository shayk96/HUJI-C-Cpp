cmake_minimum_required(VERSION 3.17)
project(ex4_shayk96 C)

set(CMAKE_C_STANDARD 11)

INCLUDE_DIRECTORIES(.)

add_executable(ex4_shayk96
        main.c
        hash_funcs.h
        hashmap.c
        hashmap.h
        pair.c
        pair.h
        test_suite.c
        test_suite.h
        test_pairs.h
        vector.c
        vector.h)