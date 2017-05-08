//
// Created by hao on 4/29/17.
//

#ifndef CG_PROJECT_SHADERFBO_H
#define CG_PROJECT_SHADERFBO_H

#include "Headers.h"

class Shadow {
public:
    Shadow(unsigned int width, unsigned int height);

    void BindForWriting();

    void BindForReading(GLenum TextureUnit);

private:
    GLuint fbo;
    GLuint shadowTex;
};


#endif //CG_PROJECT_SHADERFBO_H
