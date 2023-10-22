#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/stb_image.h"
#include "headers/SHADER.h"

#include <iostream>


const unsigned int ASPECT_RATIO[] = {16, 9};

// CALLBACKS
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void error_callback(int error, const char *description);

void setWireframeMode(int wireframeOn);
bool wireframeModeOn = false;

int main() {

    // glfw init
    glfwSetErrorCallback(error_callback);
    glfwInit();

    int count;
    int monitorX, monitorY;
    int windowWidth, windowHeight;

    GLFWmonitor **monitors = glfwGetMonitors(&count);
    const GLFWvidmode *videoMode = glfwGetVideoMode(monitors[0]);
    windowWidth = videoMode->width / 1.5;
    windowHeight = windowWidth / ASPECT_RATIO[0] * ASPECT_RATIO[1];

    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

    // WINDOW HINTS
    // -----------
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // hide window until done setting up I.E changing position etc
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    // Create Window
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "meow", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create glfw window!" << std::endl;
        glfwTerminate();
        return -1;
    }


    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowPos(window, monitorX + (videoMode->width - windowWidth) / 2, monitorY + (videoMode->height - windowHeight) / 2);
    glfwSetWindowAspectRatio(window, 16, 9);
    glfwShowWindow(window);

    // GLAD: Load OpenGL function pointers
    // -----------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, windowWidth, windowHeight);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("../../src/shaders/vertex_shader.glsl", "../../src/shaders/fragment_shader.glsl");


    // -------------------- SHADER COMPILATION END---------------------------
    // @formatter:off
    //<editor-fold desc="Vertices">
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    //</editor-fold>
    
    // @formatter:on

    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(3.0f, 2.0f, -5.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-2.5f, 2.2f, -4.5f),
            glm::vec3(2.0f, -1.0f, -2.5f),
    };

    unsigned int cubePosLength = sizeof(cubePositions) / sizeof(cubePositions[0]);

    // RENDER LOOP :3
    // --------------
    while (!glfwWindowShouldClose(window)) {
        // RENDER
        // ------
        // background color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // BIND TEXTURE
        // ------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // activate shader
        // ---------------
        ourShader.use();

        // create transformations
        // ----------------------
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float) windowWidth / (float) windowHeight, 0.1f, 100.0f);

        // pass matricies to shader
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        // render container
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < cubePosLength; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = (i + 1) * 20.0f * glfwGetTime(); // NOLINT(*-narrowing-conversions)
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // swap buffers and handle I/O
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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