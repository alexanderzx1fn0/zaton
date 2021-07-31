#ifndef SHADER_H_
#define SHADER_H_

#include "graphics/opengl/OpenGLExtensions.h"

#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <assert.h>   /* for assert */

class Shader
{
public:
    Shader(const char *path1, const char *path2);
    ~Shader();

    void bind();
    void unBind();
    GLuint getID() const { return id; }
    int getUniform ( const char* name); 

private:

    char* load_file(const char *path);
    GLuint make_shader(GLenum type, const char *source);
    GLuint load_shader(GLenum type, const char *path);
    GLuint make_program(GLuint shader1, GLuint shader2);

    GLuint id;
};


namespace shader {
GLuint load(const char * filename, GLenum shader_type, bool check_errors);

}






#endif // SHADER_H_
