//
// Created by hao on 4/26/17.
//

#include "SkyBox.h"

SkyBox::SkyBox(Camera* camera, Model *model, const vector<string> texturesPath) : cubemapTex(texturesPath) {
        this->camera = camera;
        this->model = model;
}

void SkyBox::Render(Shader &shader) {
    glDepthMask(GL_FALSE);

    shader.Use();

    mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
    mat4 projection = glm::perspective(camera->Zoom, (float)WIDTH/(float)WIDTH, NEAR, FAR);

    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // Bind Texture
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex.id);
    model->Draw(shader);

    glDepthMask(GL_TRUE);
}