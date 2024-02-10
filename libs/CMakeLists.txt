cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(FDESolverLib LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "C:/vcpkg/packages/protobuf_x64-windows/" "${CMAKE_PREFIX_PATH}")

find_package(Protobuf CONFIG REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/logger/")
include_directories("${PROJECT_SOURCE_DIR}/linalg/")
include_directories("${PROJECT_SOURCE_DIR}/utils/")
include_directories(${Protobuf_INCLUDE_DIRS})

add_subdirectory(logger)
add_subdirectory(linalg)
add_subdirectory(utils)
add_subdirectory(protos)

add_library(solvers STATIC 
    equation_solver.cpp
    mfdes.cpp
)

target_link_libraries(solvers PUBLIC logger linalg config-proto)