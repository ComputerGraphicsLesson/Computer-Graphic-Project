//
// Created by hao on 4/29/17.
//

#ifndef CG_PROJECT_SHADERFBO_H
#define CG_PROJECT_SHADERFBO_H

#include "Headers.h"
#include "Shader.h"

class Shadow {
public:
    Shadow(unsigned int width, unsigned int height);

    void Record(Shader &shadowShader, mat4 lightSpaceMatrix);

    void Finish();

    GLuint fbo;
    GLuint shadowTex;
    unsigned int shadowWidth, shadowHeight;
};


#endif //CG_PROJECT_SHADERFBO_H
