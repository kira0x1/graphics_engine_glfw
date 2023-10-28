#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"
#include "includes/SHADER.h"
#include "includes/INPUT.h"
#include "includes/CAMERA.h"
#include "level_editor.h"

#include <iostream>

const unsigned int ASPECT_RATIO[] = {16, 9};

// TIMING
// ------
float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

// CAMERA
// ------
Camera camera(glm::vec3(5.0f, 3.3f, 4.4f), glm::vec3(0.0f, 1.0f, 0.0f), -140.0f, -33.4f);
float lastX = 0.0f;
float lastY = 0.0f;
bool firstMouse = true;

// lighting
glm::vec3 lightPos(-1.2f, 0.1f, 1.5f);
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);

// plane
glm::vec3 planePos(0.0f, -0.6f, 0.0f);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void error_callback(int error, const char *description);
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
void processInput(GLFWwindow *window);
void setWireframeMode(int wireframeOn);
unsigned int loadTexture(const char *path);

bool wireframeModeOn = false;

int main() {

    LevelEditor *level = new LevelEditor("../../resources/level.txt");
    std::cout << "Level\n\n" << *level->getLevelCode() << "\n";

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
    GLFWwindow *window = glfwCreateWindow(SCRN_WDITH, SCRN_HEIGHT, "KIRλ SOURCE", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create glfw window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, monitorX + (videoMode->width - SCRN_WDITH) / 2, monitorY + (videoMode->height - SCRN_HEIGHT) / 2);
    glfwSetWindowAspectRatio(window, 16, 9);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // set callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // set icons
    GLFWimage winIcons[4];
    winIcons[0].pixels = stbi_load("../../resources/icons/gll_logo_96.png", &winIcons[0].width, &winIcons[0].height, nullptr, 4);
    winIcons[1].pixels = stbi_load("../../resources/icons/gll_logo_48.png", &winIcons[1].width, &winIcons[1].height, nullptr, 4);
    winIcons[2].pixels = stbi_load("../../resources/icons/gll_logo_32.png", &winIcons[2].width, &winIcons[2].height, nullptr, 4);
    winIcons[3].pixels = stbi_load("../../resources/icons/gll_logo_16.png", &winIcons[3].width, &winIcons[3].height, nullptr, 4);

    glfwSetWindowIcon(window, 4, winIcons);
    glfwShowWindow(window);

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

    Shader diffuseLitShader("../../src/shaders/lit/diffuse_lit_vertex.glsl", "../../src/shaders/lit/diffuse_lit_fragment.glsl");
    Shader lightingShader("../../src/shaders/lit/basic_lit_vertex.glsl", "../../src/shaders/lit/basic_lit_fragment.glsl");

    // -------------------- SHADER COMPILATION END---------------------------
    // @formatter:off
    //<editor-fold desc="Vertices">
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] =   {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    //</editor-fold>

    // @formatter:on

    unsigned int VBO;
    unsigned int cubeVAO;

    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
            glm::vec3(0.7f, 0.2f, 2.0f),
            glm::vec3(2.3f, -3.3f, -4.0f),
            glm::vec3(-4.0f, 2.0f, -12.0f),
            glm::vec3(0.0f, 0.0f, -3.0f)
    };

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap = loadTexture("../../resources/textures/container2.png");
    unsigned int specularMap = loadTexture("../../resources/textures/container2_specular.png");

    glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };

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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        diffuseLitShader.use();
        diffuseLitShader.setVec3("viewPos", camera.Position);
        diffuseLitShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index 
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        diffuseLitShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        diffuseLitShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        diffuseLitShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        diffuseLitShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        diffuseLitShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        diffuseLitShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        diffuseLitShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        diffuseLitShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        diffuseLitShader.setFloat("pointLights[0].constant", 1.0f);
        diffuseLitShader.setFloat("pointLights[0].linear", 0.09f);
        diffuseLitShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        diffuseLitShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        diffuseLitShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        diffuseLitShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        diffuseLitShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        diffuseLitShader.setFloat("pointLights[1].constant", 1.0f);
        diffuseLitShader.setFloat("pointLights[1].linear", 0.09f);
        diffuseLitShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        diffuseLitShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        diffuseLitShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        diffuseLitShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        diffuseLitShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        diffuseLitShader.setFloat("pointLights[2].constant", 1.0f);
        diffuseLitShader.setFloat("pointLights[2].linear", 0.09f);
        diffuseLitShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        diffuseLitShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        diffuseLitShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        diffuseLitShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        diffuseLitShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        diffuseLitShader.setFloat("pointLights[3].constant", 1.0f);
        diffuseLitShader.setFloat("pointLights[3].linear", 0.09f);
        diffuseLitShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        diffuseLitShader.setBool("spotLight.lightOn", false);
        diffuseLitShader.setVec3("spotLight.position", camera.Position);
        diffuseLitShader.setVec3("spotLight.direction", camera.Front);
        diffuseLitShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        diffuseLitShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        diffuseLitShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        diffuseLitShader.setFloat("spotLight.constant", 1.0f);
        diffuseLitShader.setFloat("spotLight.linear", 0.09f);
        diffuseLitShader.setFloat("spotLight.quadratic", 0.032f);
        diffuseLitShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        diffuseLitShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // view / projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) SCRN_WDITH / (float) SCRN_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        diffuseLitShader.setMat4("projection", projection);
        diffuseLitShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        diffuseLitShader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(cubeVAO);

        for (unsigned int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            diffuseLitShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // also draw the lamp object(s)
        lightingShader.use();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++) { // NOLINT(*-loop-convert)
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // swap buffers and handle I/O
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

// optional: de-allocate all resources once they've outlived their purpose:
// ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_GRAVE_ACCENT && action == GLFW_PRESS) {
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

    if (vAxis < 0) camera.Move(Camera_Movement::UP, deltaTime);
    else if (vAxis > 0) camera.Move(Camera_Movement::DOWN, deltaTime);

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
    camera.ProcessMouseScroll(static_cast<float>(yOffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}