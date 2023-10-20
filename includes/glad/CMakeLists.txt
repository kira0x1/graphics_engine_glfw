cmake_minimum_required(VERSION 3.26)
project(glad)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} include/glad/glad.h ./src/glad.c)
add_library(libs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC include)