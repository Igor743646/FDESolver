cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(FDESolver LANGUAGES C CXX)

find_package(OpenMP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG_INIT "/W3 /Od")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "/W3 /O2 /WX")

if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories("${PROJECT_SOURCE_DIR}/libs/")
add_subdirectory("${PROJECT_SOURCE_DIR}/libs/")

add_executable(main main.cpp)
target_link_libraries(main PUBLIC solvers config-proto result-proto matrix-proto linalg utils)
file(COPY "C:/vcpkg/packages/protobuf_x64-windows/bin/libprotobuf.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY "C:/vcpkg/packages/protobuf_x64-windows/bin/libprotobuf-lite.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY "C:/vcpkg/packages/protobuf_x64-windows/debug/bin/libprotobufd.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY "C:/vcpkg/packages/protobuf_x64-windows/debug/bin/libprotobuf-lited.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/libs/python")
file(COPY "libs/python/result_drawer.py" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/libs/python)