cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(AsynchronicLogger LANGUAGES CXX)

add_library(logger OBJECT 
    logger.cpp
)