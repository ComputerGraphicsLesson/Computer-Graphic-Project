//
// Created by hao on 4/26/17.
//

#ifndef CG_PROJECT_SHADOWMAPFBO_H
#define CG_PROJECT_SHADOWMAPFBO_H

#include "Headers.h"
#include "GlobeValue.h"

#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

class ScreenFBO {
public:
    ScreenFBO();
    void Record();
    void Finish();
    GLuint fbo, rbo;
    GLuint texColorBuffer;
};


#endif //CG_PROJECT_SHADOWMAPFBO_H
