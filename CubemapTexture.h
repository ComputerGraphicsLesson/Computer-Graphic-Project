//
// Created by hao on 4/26/17.
//

#ifndef CHINESE_CHESS_CUBEMAPTEXTURE_H
#define CHINESE_CHESS_CUBEMAPTEXTURE_H

#include "Headers.h"

class CubemapTexture {
public:
    CubemapTexture(const vector<string> &texturesPath);
    GLuint id;
};


#endif //CHINESE_CHESS_CUBEMAPTEXTURE_H
