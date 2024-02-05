cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(LinalgLibCTest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

include(CTest)
enable_testing()

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC linalg)

add_test(TestMatrix ${PROJECT_NAME})
