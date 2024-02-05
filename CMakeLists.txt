cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(FDESolver LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

include_directories("${PROJECT_SOURCE_DIR}/libs/")
add_subdirectory("${PROJECT_SOURCE_DIR}/libs/")

add_executable(main main.cpp)
target_link_libraries(main PUBLIC solvers)