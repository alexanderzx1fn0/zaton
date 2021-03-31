#ifndef SPRITE_H_
#define SPRITE_H_

#include "graphics/opengl/OpenGLExtensions.h"

class Sprite
{
public:
    unsigned int vao, vbo;
    Sprite();
    ~Sprite();

    void draw();
};

#endif // SPRITE_H_
