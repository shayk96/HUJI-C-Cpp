cmake_minimum_required(VERSION 3.19)
project(ex6_shayk96)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex6_shayk96
        vl_vector.h
        presubmit/presubmit.cpp
        vl_string.h)
