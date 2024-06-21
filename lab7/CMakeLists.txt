cmake_minimum_required(VERSION 3.5.1)
project(lab7 C)

include_directories(include)
set(HEADERS include/bit_set.h include/graph.h include/stack.h)
set(SRC src/bit_set.c src/graph.c src/stack.c src/main.c)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
