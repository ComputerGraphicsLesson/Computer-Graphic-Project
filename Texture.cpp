//
// Created by hao on 4/25/17.
//

#include "Texture.h"

Texture::Texture(const string &Filename, GLenum TextureTarget) {
    int width, height;
    unsigned char *image;
    // Create texture id
    glGenTextures(1, &id);
    glBindTexture(TextureTarget, id);
    // Set texture attrib
    glTexParameteri(TextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(TextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(TextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(TextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Read image
    image = SOIL_load_image(Filename.c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(TextureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free data
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}
