﻿#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/stb_image.h"
#include "headers/SHADER.h"

#include <iostream>

const unsigned int SCRN_WIDTH = 800;
const unsigned int SCRN_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void setWireframeMode(int wireframeOn);
void error_callback(int error, const char *description);

bool wireframeModeOn = false;

int main() {
    glfwSetErrorCallback(error_callback);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create Window
    GLFWwindow *window = glfwCreateWindow(SCRN_WIDTH, SCRN_HEIGHT, "meow", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create glfw window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // GLAD: Load OpenGL function pointers
    // -----------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCRN_WIDTH, SCRN_HEIGHT);

    Shader ourShader("../../src/shaders/vertex_shader.glsl", "../../src/shaders/fragment_shader.glsl");


    // -------------------- SHADER COMPILATION END---------------------------
    // @formatter:off
    float vertices[] = {
            // positions     // texture coords
             0.5f,   0.5f,  0.0f,  1.0f, 1.0f, // top right
             0.5f,  -0.5f,  0.0f,  1.0f, 0.0f, // bottom right
            -0.5f,  -0.5f,  0.0f,  0.0f, 0.0f, // bottom left
            -0.5f,   0.5f,  0.0f,  0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };
    
    // @formatter:on

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // load and create texture
    unsigned int texture1;

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set texture wrapping params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // default wrapping method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis

    unsigned char *data = stbi_load("../../resources/textures/container.jpg", &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load a texture" << std::endl;
    }

    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("containerTexture", 0);

    // RENDER LOOP :3
    // --------------
    while (!glfwWindowShouldClose(window)) {
        // RENDER
        // ------
        glClearColor(0.3f, 0.25f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        // BIND TEXTURE
        // ------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // CREATE TRANSFORMATIONS
        // ----------------------
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(0.25f, -0.25f, 0.0f));
        transform = glm::rotate(transform, (float) glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));

        // RENDER TRIANGLE
        // ---------------
        ourShader.use();

        // do transformations
        unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        // swap buffers and handle I/O
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        wireframeModeOn = !wireframeModeOn;
        std::cout << "Setting wireframe mode: " << std::boolalpha << wireframeModeOn << std::endl;
        setWireframeMode(wireframeModeOn);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        std::cout << "\nExiting via escape key\n";
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// called when window size is changed
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    // adjust viewport to match window width / height
    glViewport(0, 0, width, height);
}

void setWireframeMode(int wireframeOn) {
    if (wireframeOn) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}