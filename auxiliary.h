//
// Created by hao on 4/23/17.
//

#ifndef CG_PROJECT_AUXILIARY_H
#define CG_PROJECT_AUXILIARY_H

#include "Headers.h"
#include "Definition.h"
#include "Shader.h"
#include "Light.h"

enum Picture_Type {
    JPG,
    PNG
};



void drawObject(GLuint &VAO, const mat4 &model, Texture &diffuseTex, Texture &specularTex);


#endif //CG_PROJECT_AUXILIARY_H
