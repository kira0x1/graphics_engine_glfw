﻿#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "includes/SHADER.h"
#include "includes/INPUT.h"
#include "includes/CAMERA.h"

#include <iostream>

const unsigned int ASPECT_RATIO[] = {16, 9};

// TIMING
// ------
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// CAMERA
// ------
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

// CALLBACKS
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void error_callback(int error, const char *description);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);
void moveCamera(glm::vec3 mov);

void setWireframeMode(int wireframeOn);
bool wireframeModeOn = false;

int main() {

    // GLFW INIT
    // --------
    glfwSetErrorCallback(error_callback);
    glfwInit();

    int count;
    int monitorX, monitorY;

    GLFWmonitor **monitors = glfwGetMonitors(&count);
    const GLFWvidmode *videoMode = glfwGetVideoMode(monitors[0]);
    const int SCRN_WDITH = videoMode->width / 1.5;
    const int SCRN_HEIGHT = SCRN_WDITH / ASPECT_RATIO[0] * ASPECT_RATIO[1];

    lastX = SCRN_WDITH / 2.0f;
    lastY = SCRN_HEIGHT / 2.0f;

    glfwGetMonitorPos(monitors[0], &monitorX, &monitorY);

    // WINDOW HINTS
    // -----------
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // hide window until done setting up I.E changing position etc
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create Window
    GLFWwindow *window = glfwCreateWindow(SCRN_WDITH, SCRN_HEIGHT, "meow", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create glfw window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowPos(window, monitorX + (videoMode->width - SCRN_WDITH) / 2, monitorY + (videoMode->height - SCRN_HEIGHT) / 2);
    glfwSetWindowAspectRatio(window, 16, 9);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwShowWindow(window);

    // set icons
    GLFWimage winIcons[4];
    winIcons[0].pixels = stbi_load("../../resources/icons/gll_logo_96.png", &winIcons[0].width, &winIcons[0].height, nullptr, 4);
    winIcons[1].pixels = stbi_load("../../resources/icons/gll_logo_48.png", &winIcons[1].width, &winIcons[1].height, nullptr, 4);
    winIcons[2].pixels = stbi_load("../../resources/icons/gll_logo_32.png", &winIcons[2].width, &winIcons[2].height, nullptr, 4);
    winIcons[3].pixels = stbi_load("../../resources/icons/gll_logo_16.png", &winIcons[3].width, &winIcons[3].height, nullptr, 4);

    glfwSetWindowIcon(window, 4, winIcons);

    // free image from memory
    stbi_image_free(winIcons[0].pixels);
    stbi_image_free(winIcons[1].pixels);
    stbi_image_free(winIcons[2].pixels);
    stbi_image_free(winIcons[3].pixels);

    // GLAD: Load OpenGL function pointers
    // -----------------------------------
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCRN_WDITH, SCRN_HEIGHT);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("../../src/shaders/vertex_shader.glsl", "../../src/shaders/fragment_shader.glsl");

    // -------------------- SHADER COMPILATION END---------------------------
    // @formatter:off
    //<editor-fold desc="Vertices">
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
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

        // DELTA TIME
        // ----------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // INPUT
        processInput(window);

        // RENDER
        // ------
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // BIND TEXTURE
        // ------------
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        // ACTIVATE SHADER
        // ---------------
        ourShader.use();

        // TRANSFORMATIONS
        // ---------------

        // pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCRN_WDITH / (float) SCRN_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);

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
    if (key == GLFW_KEY_BACKSLASH && action == GLFW_PRESS) {
        wireframeModeOn = !wireframeModeOn;
        std::cout << "Setting wireframe mode: " << std::boolalpha << wireframeModeOn << std::endl;
        setWireframeMode(wireframeModeOn);
    }

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        std::cout << "\nExiting via escape key\n";
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    int leftAltState = glfwGetKey(window, GLFW_KEY_LEFT_ALT);
    int enterState = glfwGetKey(window, GLFW_KEY_ENTER);
    if (leftAltState == GLFW_PRESS && enterState == GLFW_PRESS) {
        const char *windowString = "window is windowed\nswitching to fullscreen mode";

        // returns null if windowed, and a monitor if fullscreen
        GLFWmonitor *windowMonitor = glfwGetWindowMonitor(window);
        GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);

        if (windowMonitor == nullptr) {
            windowString = "window is fullscreen\nswitching to windowed mode";
            glfwSetWindowMonitor(window, primaryMonitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
        } else {
            int monitorX, monitorY;
            int windowWidth = videoMode->width / 1.5;
            int windowHeight = windowWidth / ASPECT_RATIO[0] * ASPECT_RATIO[1];
            glfwGetMonitorPos(primaryMonitor, &monitorX, &monitorY);

            glfwSetWindowMonitor(window, nullptr, monitorX + (videoMode->width - windowWidth) / 2, monitorY + (videoMode->height - windowHeight) / 2, windowWidth, windowHeight, videoMode->refreshRate);
        }

        std::cout << windowString << std::endl;
    }
}

void processInput(GLFWwindow *window) {
    const int fwdAxis = FORWARD_AXIS.getValue(window);
    const int hAxis = HORIZONTAL_AXIS.getValue(window);
    const int vAxis = VERTICAL_AXIS.getValue(window);

    if (fwdAxis > 0) camera.Move(Camera_Movement::FORWARD, deltaTime);
    else if (fwdAxis < 0) camera.Move(Camera_Movement::BACKWARD, deltaTime);

    if (hAxis > 0) camera.Move(Camera_Movement::LEFT, deltaTime);
    else if (hAxis < 0)camera.Move(Camera_Movement::RIGHT, deltaTime);

    if (vAxis > 0) camera.Move(Camera_Movement::UP, deltaTime);
    else if (vAxis < 0) camera.Move(Camera_Movement::DOWN, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.SetBoost(true);
    } else {
        camera.SetBoost(false);
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
    fprintf(stderr, "Error: Code: %d\nDescription: %s\n", error, description);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {
    camera.ProcessMouseScroll(yOffset);
}