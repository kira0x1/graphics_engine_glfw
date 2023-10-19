cmake_minimum_required(VERSION 3.26)
project(graphics_engine_glfw)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(glfw)

set(graphics_engine_glfw main.cpp)

add_subdirectory(glad)

add_executable(graphics_engine_glfw main.cpp
        SHADER.h)
target_link_libraries(graphics_engine_glfw ${OPENGL_LIBRARIES} glfw glad)
