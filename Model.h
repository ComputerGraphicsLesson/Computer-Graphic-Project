//
// Created by hao on 4/25/17.
//

#ifndef CHINESE_CHESS_MODEL_H
#define CHINESE_CHESS_MODEL_H

#include "Headers.h"
#include "Shader.h"
#include "Mesh.h"

class Model {
public:
    Model(const string &path);
    void Draw(Shader &shader);
private:
    vector<Mesh> meshes;
    string directory;
    vector<Texture> texturesLoaded;

    void loadModel(const string &path);
    void processNode(aiNode* node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,const string &typeName);
};


#endif //CHINESE_CHESS_MODEL_H
