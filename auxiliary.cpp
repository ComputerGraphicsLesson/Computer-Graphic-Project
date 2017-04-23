//
// Created by hao on 4/23/17.
//

#include "auxiliary.h"

GLuint loadCubemap(const vector<string> &textures) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height;
    unsigned char *image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for (GLuint i = 0; i < textures.size(); i++) {
        image = SOIL_load_image(textures[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

GLuint loadTexture(const string &texture, Picture_Type type) {

    int width, height;
    unsigned char *image;

    GLuint textureID;
    glGenTextures(1, &textureID);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (type == JPG) {
        image = SOIL_load_image(texture.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    } else if (type == PNG) {
        image = SOIL_load_image(texture.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}

void drawSkybox(GLuint &VAO, Shader &shader, GLuint &skyboxTex) {
    glDepthMask(GL_FALSE);

    shader.Use();

    mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)WIDTH, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    // skybox cube
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
}

void drawObject(GLuint &VAO, Shader &shader, vec3 &lightPos,
                GLuint &diffuseTex, GLuint &specularTex) {

    shader.Use();

    mat4 projection, view, model;
    view = camera.GetViewMatrix();
    projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)WIDTH, 0.1f, 100.0f);
    GLint modelLoc = glGetUniformLocation(shader.Program, "model");      
    
    GLint viewLoc  = glGetUniformLocation(shader.Program,  "view");      
    
    GLint projLoc  = glGetUniformLocation(shader.Program,  "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTex);
    glUniform1i(glGetUniformLocation(shader.Program, "ourTex"), 0);

    GLint viewPosLoc = glGetUniformLocation(shader.Program, "viewPos");
    glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
    // Set lights properties
    glUniform3f(glGetUniformLocation(shader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
    glUniform3f(glGetUniformLocation(shader.Program, "light.ambient"),  0.2f, 0.2f, 0.2f);
    glUniform3f(glGetUniformLocation(shader.Program, "light.diffuse"),  0.5f, 0.5f, 0.5f);
    glUniform3f(glGetUniformLocation(shader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
    // Set material properties
    glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);
    glUniform1i(glGetUniformLocation(shader.Program, "material.diffuse"),  0);
    glUniform1i(glGetUniformLocation(shader.Program, "material.specular"), 1);

    glBindVertexArray(VAO);
    for(GLuint i = 0; i < 12; i++)
    {
        // Calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4();
        model = glm::translate(model, cubePositions[i]);
        model = glm::scale(model, cubeScales[i]);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
}

void drawModel(Model ourModel, Shader &shader, const mat4 &model) {
    shader.Use();
    mat4 view = camera.GetViewMatrix();
    mat4 projection = glm::perspective(camera.Zoom, (float)WIDTH/(float)WIDTH, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    ourModel.Draw(shader);
}