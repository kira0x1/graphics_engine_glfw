//
// Created by kira on 23/10/2023.
//

#ifndef KIRA_INPUT_H
#define KIRA_INPUT_H

class INPUT {
public:
    const char *name;
    [[maybe_unused]] int positiveKeys[2]{};
    [[maybe_unused]] int negativeKeys[2]{};

    explicit INPUT(const char *name, const int positiveKeys[2], const int negativeKeys[2]) {
        this->name = name;
        for (int i = 0; i < 2; i++) {
            this->positiveKeys[i] = positiveKeys[i];
            this->negativeKeys[i] = negativeKeys[i];
        }
    }
    int getValue(GLFWwindow *window) {
        int resValue = 0;

        for (int i = 0; i < 2; i++) {
            if (glfwGetKey(window, positiveKeys[i])) {
                resValue = 1;
                break;
            }
            if (glfwGetKey(window, negativeKeys[i])) {
                resValue = -1;
                break;
            }
        }

        return resValue;
    }
};

const int forwardPosKeys[2] = {GLFW_KEY_W, GLFW_KEY_UP};
const int forwardNegativeKeys[2] = {GLFW_KEY_S, GLFW_KEY_DOWN};

const int horizontalPositiveAxisKeys[2] = {GLFW_KEY_A, GLFW_KEY_LEFT};
const int horizontalNegativeAxisKeys[2] = {GLFW_KEY_D, GLFW_KEY_RIGHT};

//InputAxis HORIZONTAL_AXIS = InputAxis(nullptr, nullptr, nullptr);
INPUT HORIZONTAL_AXIS("Horizontal", horizontalPositiveAxisKeys, horizontalNegativeAxisKeys);
INPUT FORWARD_AXIS("Forward", forwardPosKeys, forwardNegativeKeys);

#endif //KIRA_INPUT_H