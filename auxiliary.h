//
// Created by hao on 4/23/17.
//

#ifndef CG_PROJECT_AUXILIARY_H
#define CG_PROJECT_AUXILIARY_H

#include "Header.h"
#include "Defination.h"
#include "Shader.h"
#include "Model.h"

enum Picture_Type {
    JPG,
    PNG
};

GLuint loadCubemap(const vector<string> &textures);
GLuint loadTexture(const string &texture, Picture_Type type);
void drawSkybox(GLuint &VAO, Shader &shader, GLuint &skyboxTex);
void drawObject(GLuint &VAO, Shader &shader, vec3 &lightPos, GLuint &diffuseTex, GLuint &specularTex);
void drawModel(Model ourModel, Shader &shader, const mat4 &model);

#endif //CG_PROJECT_AUXILIARY_H
