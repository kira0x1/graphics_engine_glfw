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
    int PositiveSize;
    int NegativeSize;

    explicit INPUT(const char *name, const int positiveKeys[2], const int negativeKeys[2], int positiveSize = 2, int negativeSize = 2) {
        this->name = name;
        PositiveSize = positiveSize;
        NegativeSize = negativeSize;

        for (int i = 0; i < positiveSize; i++) {
            this->positiveKeys[i] = positiveKeys[i];
        }

        for (int i = 0; i < negativeSize; i++) {
            this->negativeKeys[i] = negativeKeys[i];
        }
    }

    int getValue(GLFWwindow *window) {
        int resValue = 0;

        for (int i = 0; i < PositiveSize; i++) {
            if (glfwGetKey(window, positiveKeys[i])) {
                resValue = 1;
                break;
            }

        }

        for (int i = 0; i < NegativeSize; i++) {
            if (glfwGetKey(window, negativeKeys[i])) {
                resValue = -1;
                break;
            }
        }

        return resValue;
    }
};

const int forwardPositiveAxisKeys[2] = {GLFW_KEY_W, GLFW_KEY_UP};
const int forwardNegativeKeys[2] = {GLFW_KEY_S, GLFW_KEY_DOWN};

const int horizontalPositiveAxisKeys[2] = {GLFW_KEY_A, GLFW_KEY_LEFT};
const int horizontalNegativeAxisKeys[2] = {GLFW_KEY_D, GLFW_KEY_RIGHT};

const int verticalPositiveAxisKeys[1] = {GLFW_KEY_Q};
const int verticalNegativeAxisKeys[1] = {GLFW_KEY_E};

//InputAxis HORIZONTAL_AXIS = InputAxis(nullptr, nullptr, nullptr);
INPUT HORIZONTAL_AXIS("Horizontal", horizontalPositiveAxisKeys, horizontalNegativeAxisKeys, 2, 2);
INPUT FORWARD_AXIS("Forward", forwardPositiveAxisKeys, forwardNegativeKeys, 2, 2);
INPUT VERTICAL_AXIS("Vertical", verticalPositiveAxisKeys, verticalNegativeAxisKeys, 1, 1);

#endif //KIRA_INPUT_H