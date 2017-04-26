//
// Created by hao on 4/23/17.
//

#include "auxiliary.h"


void drawObject(GLuint &VAO, const mat4 &model,
                Texture &diffuseTex, Texture &specularTex) {
    LightHandler::light->Use(model);
    LightHandler::light->ChangeMaterial(32, diffuseTex, specularTex);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}
