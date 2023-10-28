cmake_minimum_required(VERSION 3.26)
project(src)

add_executable(${PROJECT_NAME} main.cpp includes/SHADER.h includes/INPUT.h includes/CAMERA.h
        level_editor.cpp
        level_editor.h
        level_editor.h)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw glad glm stb)