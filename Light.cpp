//
// Created by hao on 4/23/17.
//

#include "Light.h"

Light::Light(Camera *camera) : shader("Shaders/lightShader.vs", "Shaders/lightShader.frag"){
    this->camera = camera;
}
void Light::ChangeDirLight(vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular)
{
//    shader.Use();
    glUniform1i(glGetUniformLocation(shader.Program, "dirLightEnable"), 0);
    glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), direction.x, direction.y, direction.z);
    glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), specular.x, specular.y, specular.z);
}

void Light::AddPointLight(vec3 position, vec3 ambient,
                          vec3 diffuse, vec3 specular,
                          float constant, float linear, float quadratic)
{
//    shader.Use();
    char attrib[256];
    snprintf(attrib, 256, "pointLights[%d].position", (int)pointPositions.size());
    glUniform3f(glGetUniformLocation(shader.Program, attrib), position.x, position.y, position.z);
    snprintf(attrib, 256, "pointLights[%d].ambient", (int)pointPositions.size());
    glUniform3f(glGetUniformLocation(shader.Program, attrib), ambient.x, ambient.y, ambient.z);
    snprintf(attrib, 256, "pointLights[%d].diffuse", (int)pointPositions.size());
    glUniform3f(glGetUniformLocation(shader.Program, attrib), diffuse.x, diffuse.y, diffuse.z);
    snprintf(attrib, 256, "pointLights[%d].specular", (int)pointPositions.size());
    glUniform3f(glGetUniformLocation(shader.Program, attrib), specular.x, specular.y, specular.z);
    snprintf(attrib, 256, "pointLights[%d].constant", (int)pointPositions.size());
    glUniform1f(glGetUniformLocation(shader.Program, attrib), constant);
    snprintf(attrib, 256, "pointLights[%d].linear", (int)pointPositions.size());
    glUniform1f(glGetUniformLocation(shader.Program, attrib), linear);
    snprintf(attrib, 256, "pointLights[%d].quadratic", (int)pointPositions.size());
    glUniform1f(glGetUniformLocation(shader.Program, attrib), quadratic);
    pointPositions.push_back(position);
}

void Light::ChangeSpotLight(vec3 ambient, vec3 diffuse, vec3 specular,
                            float cutoff, float outerCutOff, float constant,
                            float linear, float quadratic) {
//    shader.Use();
    glUniform3f(glGetUniformLocation(shader.Program, "spotLight.ambient"), ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(shader.Program, "spotLight.diffuse"), diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(shader.Program, "spotLight.specular"), specular.x, specular.y, specular.z);
    glUniform1f(glGetUniformLocation(shader.Program, "spotLight.constant"), constant);
    glUniform1f(glGetUniformLocation(shader.Program, "spotLight.linear"), linear);
    glUniform1f(glGetUniformLocation(shader.Program, "spotLight.quadratic"), quadratic);
    glUniform1f(glGetUniformLocation(shader.Program, "spotLight.cutOff"), glm::cos(glm::radians(cutoff)));
    glUniform1f(glGetUniformLocation(shader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(outerCutOff)));
}

void Light::ChangeMaterial(float shininess, Texture &diffTex, Texture &specularTex) {
//    shader.Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffTex.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTex.id);

    // Set material properties
    glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), shininess);
    glUniform1i(glGetUniformLocation(shader.Program, "material.diffuse"),  0);
    glUniform1i(glGetUniformLocation(shader.Program, "material.specular"), 1);
}

void Light::Use(const mat4 &model) {
    shader.Use();
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera->Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, NEAR, FAR);
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glUniform3f(glGetUniformLocation(shader.Program, "spotLight.position"), camera->Position.x, camera->Position.y, camera->Position.z);
    glUniform3f(glGetUniformLocation(shader.Program, "spotLight.direction"), camera->Front.x, camera->Front.y, camera->Front.z);
}

void Light::Draw(GLuint &VAO, Shader& shader) {
    shader.Use();
    mat4 view = camera->GetViewMatrix();
    mat4 projection = glm::perspective(camera->Zoom, (float) WIDTH / (float) WIDTH, NEAR, FAR);
    glUniform3f(glGetUniformLocation(shader.Program, "viewPos"), camera->Position.x,
                camera->Position.y, camera->Position.z);
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // Draw Cube
    mat4 model;
    glBindVertexArray(VAO);
    for (int i = 0; i < pointPositions.size(); i++) {
        model = glm::translate(mat4(), pointPositions[i]);
        model = glm::scale(model, vec3(4, 4, 4));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
}
