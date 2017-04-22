//
// Created by hao on 4/22/17.
//

#ifndef CG_PROJECT_DEFINATION_H
#define CG_PROJECT_DEFINATION_H

#include "Header.h"
#include "Camera.h"

extern const GLuint WIDTH;
extern const GLuint HEIGHT;

extern double lastX, lastY;

extern const char objVSPath[];
extern const char objFragPath[];
extern const char lampVSPath[];
extern const char lampFragPath[];
extern const char wallPicPath[];
extern const char skyPicPath[];

extern Camera camera;

extern bool keys[1024];

extern const GLfloat cube[576];

extern glm::vec3 cubePositions[];
extern glm::vec3 cubeScales[];

#endif //CG_PROJECT_DEFINATION_H
