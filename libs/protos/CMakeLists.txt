cmake_minimum_required (VERSION 3.16)
project(ProtoLib)

find_package(Protobuf REQUIRED CONFIG)

set(PROTO_OUTPUT_DIR_CXX "${CMAKE_CURRENT_BINARY_DIR}/sources")
set(PROTO_OUTPUT_DIR_PY "${CMAKE_CURRENT_BINARY_DIR}/../python")
set(Protobuf_USE_STATIC_LIBS "ON" "${Protobuf_USE_STATIC_LIBS}")
message("Proto files output dir: ${PROTO_OUTPUT_DIR}")

list(APPEND PROTO_FILES
    "${CMAKE_CURRENT_LIST_DIR}/config.proto"
)

message("Generate proto files...")
foreach(proto ${PROTO_FILES})
    get_filename_component(PROTO_NAME_WE ${proto} NAME_WE)
    message("Proto file : ${proto}")
    message("Proto file name: ${PROTO_NAME_WE}")
    
    message("Adding library: ${PROTO_NAME_WE}-proto...")
    add_library("${PROTO_NAME_WE}-proto" STATIC OBJECT ${proto})

    protobuf_generate(TARGET "${PROTO_NAME_WE}-proto" 
                      PROTOC_OUT_DIR "${PROTO_OUTPUT_DIR_CXX}"
                      IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}")

    protobuf_generate(TARGET "${PROTO_NAME_WE}-proto" 
                      LANGUAGE python
                      PROTOC_OUT_DIR "${PROTO_OUTPUT_DIR_PY}"
                      IMPORT_DIRS "${CMAKE_CURRENT_LIST_DIR}")

    target_link_libraries("${PROTO_NAME_WE}-proto" PUBLIC 
                                                          protobuf::libprotoc 
                                                          protobuf::libprotobuf 
                                                          protobuf::libprotobuf-lite)
    target_include_directories("${PROTO_NAME_WE}-proto" PUBLIC "$<BUILD_INTERFACE:${PROTO_OUTPUT_DIR_CXX}>")
endforeach()
