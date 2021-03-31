#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "graphics/opengl/OpenGLExtensions.h"

#include <stdio.h>

class Texture {
public:
    unsigned int id;        // OpenGL texture id
    int width;              // Texture base width
    int height;             // Texture base height
    int mipmaps;            // Mipmap levels, 1 by default
    GLenum format;             // Data format (TextureFormat)
    int components;
    

    Texture(const char *filename);
    ~Texture();

    void bind(int sampler);
};


#endif //TEXTURE_H_
