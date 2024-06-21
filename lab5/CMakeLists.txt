cmake_minimum_required(VERSION 3.5.1)
project(lab5 C)

set(HEADERS
    include/bit_reader.h
    include/bit_set.h
    include/bit_writer.h
    include/decode.h
    include/encode.h
    include/huffman.h
    include/priority_queue.h)
set(SRC
    src/bit_reader.c
    src/bit_set.c
    src/bit_writer.c
    src/decode.c
    src/encode.c
    src/priority_queue.c
    src/main.c)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE include)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
