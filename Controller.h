//
// Created by hao on 4/25/17.
//

#ifndef COMPUTER_GRAPHIC_PROJECT_CONTROLLER_H
#define COMPUTER_GRAPHIC_PROJECT_CONTROLLER_H

#include "Headers.h"

enum Function_Type {
    SKYBOX,
    MODEL,
    SPOTLIGHT,
    POINTLIGHT,
    DIRLIGHT
};

class Controller {
public:
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void Movement(double deltaTime);
    static bool GetFunctionState(Function_Type functionType);

private:
    static bool firstMouse;
    static bool keys[1024];
    static double lastX, lastY;
    // Function State
    static bool skyboxEnable;
    static bool modelEnable;
    static bool spotLightEnable;
    static bool pointLightEnable;
    static bool dirLightEnable;
};


#endif //COMPUTER_GRAPHIC_PROJECT_CONTROLLER_H
