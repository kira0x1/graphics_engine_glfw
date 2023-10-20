cmake_minimum_required(VERSION 3.26)
project(src)

add_executable(${PROJECT_NAME} main.cpp headers/SHADER.h headers/stb_image.h
        stb_image.cpp)

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw glad glm)