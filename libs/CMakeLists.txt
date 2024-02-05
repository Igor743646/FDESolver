cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(FDESolverLib LANGUAGES CXX)

message(${PROJECT_SOURCE_DIR})

include_directories("${PROJECT_SOURCE_DIR}/logger/")
add_subdirectory("logger/")

include_directories("${PROJECT_SOURCE_DIR}/linalg/")
add_subdirectory("linalg/")

include_directories("${PROJECT_SOURCE_DIR}/utils/")
add_subdirectory("utils/")

add_library(solvers STATIC 
    equation_solver.cpp
    mfdes.cpp
)

target_link_libraries(solvers PUBLIC logger linalg)