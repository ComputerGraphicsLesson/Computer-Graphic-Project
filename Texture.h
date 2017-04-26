//
// Created by hao on 4/25/17.
//

#ifndef COMPUTER_GRAPHIC_PROJECT_TEXTURE_H
#define COMPUTER_GRAPHIC_PROJECT_TEXTURE_H

#include <GL/glew.h>
#include "Headers.h"

class Texture {
public:
    Texture(const string &Filename, GLenum TextureTarget = GL_TEXTURE_2D);
    string type;
    aiString path;
    GLuint id;
};


#endif //COMPUTER_GRAPHIC_PROJECT_TEXTURE_H
