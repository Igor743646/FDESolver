cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(FDESolverLib LANGUAGES CXX)

find_package(Protobuf CONFIG REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/logger/")
include_directories("${PROJECT_SOURCE_DIR}/linalg/")
include_directories("${PROJECT_SOURCE_DIR}/utils/")
include_directories(${Protobuf_INCLUDE_DIRS})

add_subdirectory(logger)
add_subdirectory(linalg)
add_subdirectory(utils)
add_subdirectory(protos)

add_library(solvers STATIC OBJECT 
    equation_solver.cpp
    mfdes.cpp
)

target_link_libraries(solvers PUBLIC logger linalg config-proto matrix-proto)