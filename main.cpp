#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SHADER.h"

#include <iostream>
#include <cmath>

const unsigned int SCRN_WIDTH = 800;
const unsigned int SCRN_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void setWireframeMode(int wireframeOn);

bool wireframeModeOn = false;

int main() {
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

    Shader ourShader("../vertex_shader.glsl", "../fragment_shader.glsl");

    // -------------------- SHADER COMPILATION END---------------------------
    // @formatter:off
    float vertices[] = {
            // positions            //colors
             0.5f,  -0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   // bottom right
            -0.5f,  -0.5f,  0.0f,   0.0f, 1.0f, 0.0f,   // bottom left
             0.0f,   0.5f,  0.0f,   0.0f, 0.0f, 1.0f,   // top
    };
    // @formatter:on

    unsigned int VBO;
    unsigned int VAO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

//    setWireframeMode(wireframeModeOn);

    // RENDER LOOP :3
    // --------------
    while (!glfwWindowShouldClose(window)) {

        // INPUT
        // -----
        processInput(window);

        // RENDER
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



//        // UPDATE UNIFORM COLOR
//        // --------------------
//        double timeValue = glfwGetTime();
//        float greenValue = static_cast<float>(sin(timeValue) / 2.0f + 0.5f);
//        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
//        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);


        // RENDER TRIANGLE
        // ---------------
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "\nExiting via escape key\n";
        glfwSetWindowShouldClose(window, true);
    }
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        wireframeModeOn = !wireframeModeOn;
        std::cout << "Setting wireframe mode: " << std::boolalpha << wireframeModeOn << std::endl;
        setWireframeMode(wireframeModeOn);
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