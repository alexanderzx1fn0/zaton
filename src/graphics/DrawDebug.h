#ifndef DRAW_DEBUG_H_
#define DRAW_DEBUG_H_

#include "util/utils.h"
#include "graphics/opengl/OpenGLExtensions.h"

#include "graphics/Shader.h"

class Line
{
public:
    Line();
    ~Line();
    void create();
    void draw(const vec3& ro, const vec3& rd, float t);


    GLuint vao;
    GLuint vbo;

    float vertices[6];

    void setUniform(const mat4* matViewProj, const mat4* model);

    class Shader* program;
};

#endif // DRAW_DEBUG_H_
