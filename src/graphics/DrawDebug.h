#ifndef DRAW_DEBUG_H_
#define DRAW_DEBUG_H_

#include "util/utils.h"
#include "graphics/opengl/OpenGLExtensions.h"

struct Line
{
    void create();
    void draw(const vec3& ro, const vec3& rd, float t);


    GLuint vao;
    GLuint vbo;

    float vertices[6];
};

#endif // DRAW_DEBUG_H_
