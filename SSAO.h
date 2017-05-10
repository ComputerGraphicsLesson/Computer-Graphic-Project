//
// Created by hao on 5/10/17.
//

#ifndef CG_PROJECT_SSAO_H
#define CG_PROJECT_SSAO_H

#include "Headers.h"
#include "Definition.h"

class SSAO {
public:
    SSAO();
    void RecordSSAO();
    void FinishSSAO();
    void RecordBlur();
    void FinishBlur();
    GLuint ssaoFBO, ssaoBlurFBO;
    GLuint ssaoColorTex, ssaoColorBlurTex, noiseTex;
    std::vector<glm::vec3> ssaoKernel;
    std::vector<glm::vec3> ssaoNoise;
private:
    GLfloat lerp(GLfloat a, GLfloat b, GLfloat f);

};


#endif //CG_PROJECT_SSAO_H
