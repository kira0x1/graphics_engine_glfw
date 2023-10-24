cmake_minimum_required(VERSION 3.26)
project(src)

add_executable(${PROJECT_NAME} main.cpp includes/SHADER.h includes/INPUT.h includes/CAMERA.h)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw glad glm stb)