cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(LinalgLib LANGUAGES CXX)

add_library(linalg STATIC 
    matrix.cpp
)

target_link_libraries(linalg PUBLIC utils)
