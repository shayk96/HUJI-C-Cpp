cmake_minimum_required(VERSION 3.19)
project(ex5_shayk96)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(ex5_shayk96
        Matrix.cpp
        Matrix.h
        Activation.cpp
        Activation.h
        Dense.cpp
        Dense.h
        MlpNetwork.cpp
        MlpNetwork.h
        main.cpp)
