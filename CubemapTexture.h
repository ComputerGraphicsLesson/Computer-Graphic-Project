//
// Created by hao on 4/26/17.
//

#ifndef CHINESE_CHESS_CUBEMAPTEXTURE_H
#define CHINESE_CHESS_CUBEMAPTEXTURE_H

#include "Headers.h"
//"Textures/SkyBoxs/right.jpg",
//"Textures/SkyBoxs/left.jpg",
//"Textures/SkyBoxs/top.jpg",
//"Textures/SkyBoxs/bottom.jpg",
//"Textures/SkyBoxs/back.jpg",
//"Textures/SkyBoxs/front.jpg"
class CubemapTexture {
public:
    CubemapTexture(const vector<string> &texturesPath);
    GLuint textureId;
};


#endif //CHINESE_CHESS_CUBEMAPTEXTURE_H
