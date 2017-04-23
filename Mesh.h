//
// Created by hao on 4/23/17.
//

#ifndef CG_PROJECT_MESH_H
#define CG_PROJECT_MESH_H

#include "Header.h"
#include "Shader.h"
#include "Defination.h"


class Mesh {
public:
    vector<Vertex> vertices;
    vector<GLuint> indices;
    vector<Texture> textures;
    Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> texture);
    void Draw(Shader shader);

private:
    GLuint VAO, VBO, EBO;
    void setupMesh();
};

#endif //CG_PROJECT_MESH_H
