////
//// Created by hao on 4/23/17.
////
//
//#include "Object.h"
//
//Object::Object(vector<Vertex> &vertices, vector<Texture> &texture, vector<vec3> &positions) {
//    this->vertices = vertices;
//    this->textures = textures;
//    this->positions = positions;
//    this->setupMesh();
//}
//
//Object::~Object() {
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//}
//
//void Object::setupMesh() {
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
//    // Vertex Position
//    glEnableVertexAttribArray(0);
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
//    // Vertex Normals
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
//    // Vertex Texture Coords
//    glEnableVertexAttribArray(2);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
//
//    glBindVertexArray(0);
//}
//
//void Object::Draw(Shader &shader, Camera &camera, vec3 &lightPos) {
//
//    mat4 projection, view, model;
//
//    GLint objectColorLoc = glGetUniformLocation(shader.Program, "objectColor");
//    GLint lightColorLoc  = glGetUniformLocation(shader.Program, "lightColor");
//    GLint lightPosLoc    = glGetUniformLocation(shader.Program, "lightPos");
//    GLint viewPosLoc     = glGetUniformLocation(shader.Program, "viewPos");
//
//    glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
//    glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
//    glUniform3f(lightPosLoc,    lightPos.x, lightPos.y, lightPos.z);
//    glUniform3f(viewPosLoc,     camera.Position.x, camera.Position.y, camera.Position.z);
//
//    GLint modelLoc = glGetUniformLocation(shader.Program, "model");
//    GLint viewLoc = glGetUniformLocation(shader.Program, "view");
//    GLint projLoc = glGetUniformLocation(shader.Program, "projection");
//
//    view = camera.GetViewMatrix();
//    projection = perspective(camera.Zoom, (float)WIDTH/(float)HEIGHT, 0.1f, 1000.0f);
//
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
//    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
//
//    glBindVertexArray(VAO);
//    for (int i = 0; i < positions.size(); i++) {
//        model = glm::mat4();
//        model = glm::translate(model, cubePositions[i]);
//        model = glm::scale(model, cubeScales[i]);
//        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//    }
//    glBindVertexArray(0);
//
//    for (GLuint i = 0; i < textures.size(); i++)
//    {
//        glActiveTexture(GL_TEXTURE0 + i);
//        glBindTexture(GL_TEXTURE_2D, 0);
//    }
//}