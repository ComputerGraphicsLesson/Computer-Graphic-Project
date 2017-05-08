//
// Created by hao on 4/25/17.
//

#include "Controller.h"
#include "GlobeValue.h"

double Controller::lastX = WIDTH;
double Controller::lastY = HEIGHT;
bool Controller::firstMouse = true;
bool Controller::keys[1024] = {false};
bool Controller::skyboxEnable = true;
bool Controller::modelEnable = false;
bool Controller::spotLightEnable = false;
bool Controller::pointLightEnable = false;
bool Controller::dirLightEnable = false;

void Controller::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        skyboxEnable = !skyboxEnable;
    }
    if (key == GLFW_KEY_I && action == GLFW_PRESS) {
        spotLightEnable = !spotLightEnable;
    }
    if (key == GLFW_KEY_O && action == GLFW_PRESS) {
        dirLightEnable = !dirLightEnable;
    }
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        pointLightEnable = !pointLightEnable;
    }
}

void Controller::MouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (__glibc_unlikely(firstMouse)) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    mainCamera->ProcessMouseMovement((GLfloat) (xpos - lastX), (GLfloat) (lastY - ypos));

    lastX = xpos;
    lastY = ypos;
}

void Controller::Movement(double deltaTime) {
    // Camera controls
    if(keys[GLFW_KEY_W])
        mainCamera->ProcessKeyboard(CAM_FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        mainCamera->ProcessKeyboard(CAM_BACKWARD, deltaTime);
    if(keys[GLFW_KEY_A])
        mainCamera->ProcessKeyboard(CAM_LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        mainCamera->ProcessKeyboard(CAM_RIGHT, deltaTime);
}

bool Controller::GetFunctionState(Function_Type functionType) {
    switch (functionType) {
        case SKYBOX:
            if (skyboxEnable)
                return true;
            break;
        case MODEL:
            if (modelEnable)
                return true;
            break;
        case SPOTLIGHT:
            if (spotLightEnable)
                return true;
            break;
        case POINTLIGHT:
            if (pointLightEnable)
                return true;
            break;
        case DIRLIGHT:
            if (dirLightEnable)
                return true;
            break;
    }
    return false;
}