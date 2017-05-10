//
// Created by hao on 5/9/17.
//

#ifndef CG_PROJECT_GBUFFER_H
#define CG_PROJECT_GBUFFER_H

#include "Headers.h"
#include "Definition.h"


class GBuffer {
public:
    GBuffer();
    void Record();
    void Finish();
    GLuint gBuffer;
    GLuint gPositionDepth, gNormal, gAlbedo;

};


#endif //CG_PROJECT_GBUFFER_H
