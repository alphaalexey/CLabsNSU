cmake_minimum_required(VERSION 3.5.1)
project(lab10 C)

set(HEADERS include/graham.h include/stack.h)
set(SRC src/graham.c src/stack.c src/main.c)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS})
target_compile_definitions(${PROJECT_NAME} PRIVATE _GNU_SOURCE=1)
target_include_directories(${PROJECT_NAME} PRIVATE include)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../modules")
include(common_lab)
