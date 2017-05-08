//
// Created by hao on 4/23/17.
//

#ifndef CG_PROJECT_LIGHT_H
#define CG_PROJECT_LIGHT_H

#include "Headers.h"
#include "Shader.h"
#include "Definition.h"
#include "Texture.h"
#include "Camera.h"

enum Light_Type {
    POINT_LIGHT,
    DIRECTIONAL_LIGHT,
    SPOT_LIGHT
};

class Light {
public:
    Light(Camera *camera);
    void ChangeDirLight(vec3 direction, vec3 ambient, vec3 diffuse, vec3 specular);

    void AddPointLight(vec3 position, vec3 ambient,
                       vec3 diffuse, vec3 specular,
                       float constant, float linear, float quadratic);

    void ChangeSpotLight(vec3 ambient, vec3 diffuse, vec3 specular,
                         float cutoff, float outerCutOff, float constant,
                         float linear, float quadratic);

    void ChangeMaterial(float shininess, Texture &diffTex, Texture &specularTex);

    void Draw(GLuint &VAO, Shader& shader);

    void Use(const mat4 &model);

    Shader shader;

private:
    Camera *camera;
    vector<vec3> pointPositions;
};

#endif //CG_PROJECT_LIGHT_H
