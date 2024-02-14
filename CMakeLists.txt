cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(FDESolver LANGUAGES C CXX)

# C++ flags
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG_INIT "/W3 /Od")
set(CMAKE_CXX_FLAGS_RELEASE_INIT "/W3 /O2 /WX")
# Protobuf path //TODO for linux
set(CMAKE_PREFIX_PATH "C:/vcpkg/packages/protobuf_x64-windows/" "${CMAKE_PREFIX_PATH}")

find_package(Protobuf REQUIRED CONFIG)

if (PROTOBUF_FOUND)
    message(STATUS "Protobuf found: ${Protobuf_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "NO Protobuf: you make install it using vcpkg for Windows: 
    https://github.com/protocolbuffers/protobuf/blob/main/src/README.md#c-protobuf---windows")
endif()

include_directories("${PROJECT_SOURCE_DIR}/libs/")
add_subdirectory("${PROJECT_SOURCE_DIR}/libs/")

add_executable(main main.cpp)
target_link_libraries(main PUBLIC solvers config-proto result-proto matrix-proto linalg utils)

# Copy dll's for protobuf
add_custom_command(TARGET main POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:main> $<TARGET_RUNTIME_DLLS:main>
  COMMAND_EXPAND_LISTS
  COMMENT "Copy dll's to binary ($<TARGET_FILE_DIR:main>/$<TARGET_FILE_NAME:main>) list: ($<TARGET_RUNTIME_DLLS:main>)"
)
