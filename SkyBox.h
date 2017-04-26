//
// Created by hao on 4/26/17.
//

#ifndef CHINESE_CHESS_SKYBOX_H
#define CHINESE_CHESS_SKYBOX_H

#include "Headers.h"
#include "Camera.h"
#include "CubemapTexture.h"
#include "Shader.h"
#include "Definition.h"
#include "Mesh.h"
#include "Model.h"

class SkyBox {
public:
    SkyBox(Camera* camera, Model *model, const vector<string> texturesPath);

    void Draw(Shader &shader);

private:
    Camera* camera;
    Model* model;
    CubemapTexture cubemapTex;
};


#endif //CHINESE_CHESS_SKYBOX_H
