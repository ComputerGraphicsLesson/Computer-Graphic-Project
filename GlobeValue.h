//
// Created by hao on 4/22/17.
//

#ifndef CG_PROJECT_DEFINATION_H
#define CG_PROJECT_DEFINATION_H

#include "Headers.h"
#include "Camera.h"
#include "Shader.h"
#include "Light.h"

// Cube demo
extern const GLfloat cube[576];
extern glm::vec3 cubePositions[16];
extern glm::vec3 cubeScales[16];
extern GLfloat quadVertices[24];
extern GLfloat planeVertices[48];

extern Camera *mainCamera;

#endif //CG_PROJECT_DEFINATION_H
