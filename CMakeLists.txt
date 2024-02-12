cmake_minimum_required(VERSION 3.26 FATAL_ERROR)
project(FDESolver LANGUAGES C CXX)

set(CMAKE_PREFIX_PATH "C:/vcpkg/packages/protobuf_x64-windows/" "${CMAKE_PREFIX_PATH}")

find_package(Protobuf REQUIRED CONFIG)

if (PROTOBUF_FOUND)
    list(GET Protobuf_LIBRARIES 1 Protobuf_libprotobuf_DLL)
    list(GET Protobuf_LIBRARIES 3 Protobuf_libprotobufd_DLL)
    list(GET Protobuf_LITE_LIBRARIES 1 Protobuf_libprotobuf_lite_DLL)
    list(GET Protobuf_LITE_LIBRARIES 3 Protobuf_libprotobuf_lited_DLL)
    set(BUILD_libprotobuf_DLL "$<IF:$<CONFIG:Debug>,${Protobuf_libprotobufd_DLL},${Protobuf_libprotobuf_DLL}>")
    set(BUILD_libprotobuf_lite_DLL "$<IF:$<CONFIG:Debug>,${Protobuf_libprotobuf_lited_DLL},${Protobuf_libprotobuf_lite_DLL}>")
endif()

set(FULL_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
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

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${BUILD_libprotobuf_DLL}" "${FULL_OUTPUT_DIR}/$<CONFIG>"
    COMMENT "Copy dll ${BUILD_libprotobuf_DLL} in ${FULL_OUTPUT_DIR}/$<CONFIG> \n"
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${BUILD_libprotobuf_lite_DLL}" "${FULL_OUTPUT_DIR}/$<CONFIG>"
    COMMENT "Copy dll ${BUILD_libprotobuf_lite_DLL} in ${FULL_OUTPUT_DIR}/$<CONFIG> \n"
)

add_custom_command(
    TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/libs/python/result_drawer.py" "${CMAKE_CURRENT_BINARY_DIR}/libs/python"
    COMMENT "Copy result_drawer.py in ${CMAKE_CURRENT_BINARY_DIR}/libs/python \n"
)
