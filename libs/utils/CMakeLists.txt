cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(Utils LANGUAGES CXX)

include_directories("${PROJECT_SOURCE_DIR}/../logger/")

add_library(utils STATIC 
    utils.cpp
)

target_link_libraries(utils PUBLIC logger)