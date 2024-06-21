cmake_minimum_required(VERSION 3.5.1)
project(lab6-2 C)

set(HEADERS include/btree.h)
set(SRC src/btree.c src/main.c)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE include)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
