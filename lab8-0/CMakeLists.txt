cmake_minimum_required(VERSION 3.5.1)
project(lab8-0 C)

include_directories(include)
set(HEADERS include/dsu.h)
set(SRC src/dsu.c src/main.c)

add_executable(${PROJECT_NAME} ${SRC})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
