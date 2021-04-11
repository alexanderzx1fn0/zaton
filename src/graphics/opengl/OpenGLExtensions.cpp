#include "OpenGLExtensions.h"

#if defined(_WIN32)
void* GetProc(const char *name) {
    return (void*)wglGetProcAddress(name);
}

#define GetProcGL(x) x = (decltype(x))GetProc(#x);

#elif defined(__linux__)
#include <GL/glx.h>

void* GetProc(const char *name) {
    return (void*)glXGetProcAddress((const GLubyte*)name);
}

#define GetProcGL(x) x = (decltype(x))GetProc(#x);

#endif

#define LOG(...) printf(__VA_ARGS__)

#if defined(_WIN32)
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
PFNGLACTIVETEXTUREPROC  glActiveTexture = NULL;
#endif

PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
PFNGLGENBUFFERSPROC     glGenBuffers = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;

PFNGLCREATEPROGRAMPROC              glCreateProgram = NULL;
PFNGLCREATESHADERPROC               glCreateShader = NULL;
PFNGLSHADERSOURCEPROC               glShaderSource = NULL;
PFNGLCOMPILESHADERPROC              glCompileShader = NULL;
PFNGLGETSHADERIVPROC                glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog = NULL;
PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog = NULL;
PFNGLATTACHSHADERPROC               glAttachShader = NULL;
PFNGLLINKPROGRAMPROC                glLinkProgram = NULL;
PFNGLGETPROGRAMIVPROC               glGetProgramiv = NULL;
PFNGLDETACHSHADERPROC               glDetachShader = NULL;
PFNGLDELETESHADERPROC               glDeleteShader = NULL;
PFNGLUSEPROGRAMPROC                 glUseProgram = NULL;
PFNGLUNIFORM4FVPROC                 glUniform4fv = NULL;
PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation = NULL;
PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv = NULL;
PFNGLUNIFORM1IPROC		    glUniform1i = NULL;
PFNGLUNIFORM3FVPROC                 glUniform3fv = NULL;
PFNGLUNIFORM1FPROC                  glUniform1f = NULL;

PFNGLDELETEPROGRAMPROC              glDeleteProgram = NULL;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = NULL;

/** BUFFERS **/
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLBINDBUFFERPROC     glBindBuffer = NULL;
PFNGLBUFFERDATAPROC     glBufferData = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer = NULL;
PFNGLDELETEBUFFERSPROC  glDeleteBuffers = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;

void init_opengl_extensions() {
        LOG("Vendor   : %s\n", glGetString(GL_VENDOR));
        LOG("Renderer : %s\n", glGetString(GL_RENDERER));
        LOG("Version  : %s\n", glGetString(GL_VERSION));

        GetProcGL(glGenFramebuffers);
        GetProcGL(glGenBuffers);
        GetProcGL(glCreateProgram);
        GetProcGL(glCreateShader);
        GetProcGL(glShaderSource);
        GetProcGL(glCompileShader);
        GetProcGL(glGetShaderiv);
        GetProcGL(glGetShaderInfoLog);
        GetProcGL(glGetProgramInfoLog);
        GetProcGL(glAttachShader);
        GetProcGL(glLinkProgram);
        GetProcGL(glGetProgramiv);
        GetProcGL(glDetachShader);
        GetProcGL(glDeleteShader);
        GetProcGL(glUseProgram);
        GetProcGL(glUniform4fv);
        GetProcGL(glGetUniformLocation);
        GetProcGL(glUniformMatrix4fv);
        GetProcGL(glGenVertexArrays);
        GetProcGL(glBindVertexArray);
        GetProcGL(glBindBuffer);
        GetProcGL(glBufferData);
        GetProcGL(glEnableVertexAttribArray);
        GetProcGL(glVertexAttribPointer);
        GetProcGL(glDeleteBuffers);
        GetProcGL(glDeleteVertexArrays);
        GetProcGL(glDeleteProgram);
        GetProcGL(glGenerateMipmap);
	GetProcGL(glUniform1i);
	GetProcGL(glBufferSubData);
        GetProcGL(glUniform3fv);
        GetProcGL(glUniform1f);
#if defined(_WIN32)
	GetProcGL(glActiveTexture);
#endif
}

