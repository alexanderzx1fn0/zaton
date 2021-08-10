#include "OpenGLExtensions.h"

#define LOG(...) printf(__VA_ARGS__)

#if defined(_WIN32)
PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = NULL;
PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = NULL;
PFNGLACTIVETEXTUREPROC  glActiveTexture = NULL;
PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D = NULL;
#endif
PFNGLMAPBUFFERPROC glMapBuffer = NULL;
PFNGLUNMAPBUFFERPROC glUnmapBuffer = NULL;

PFNGLUNIFORM1IVPROC glUniform1iv = NULL;
PFNGLUNIFORM4IVPROC glUniform4iv = NULL;
PFNGLUNIFORM2IVPROC glUniform2iv = NULL;
PFNGLUNIFORM1FVPROC glUniform1fv = NULL;
PFNGLUNIFORM2FVPROC glUniform2fv = NULL;

PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform = NULL;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = NULL;
PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor = NULL;

PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced = NULL;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers = NULL;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus = NULL;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D = NULL;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers = NULL;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer = NULL;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers = NULL;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer = NULL;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage = NULL;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer = NULL;
PFNGLDRAWBUFFERSPROC glDrawBuffers = NULL;


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
PFNGLUNIFORM3FPROC                  glUniform3f = NULL;
PFNGLUNIFORM4FPROC                  glUniform4f = NULL;

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

#if defined(__linux__)
    glMapBuffer = (PFNGLMAPBUFFERPROC)glXGetProcAddress((const GLubyte*)"glMapBuffer");
    glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)glXGetProcAddress((const GLubyte*)"glUnmapBuffer");

		glUniform1iv = (PFNGLUNIFORM1IVPROC)glXGetProcAddress((const GLubyte*)"glUniform1iv");
		glUniform4iv = (PFNGLUNIFORM4IVPROC)glXGetProcAddress((const GLubyte*)"glUniform4iv");
		glUniform2iv = (PFNGLUNIFORM2IVPROC)glXGetProcAddress((const GLubyte*)"glUniform2iv");
		glUniform1fv = (PFNGLUNIFORM1FVPROC)glXGetProcAddress((const GLubyte*)"glUniform1fv");
		glUniform2fv = (PFNGLUNIFORM2FVPROC)glXGetProcAddress((const GLubyte*)"glUniform2fv");

		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)glXGetProcAddress((const GLubyte*)"glGetActiveUniform");
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glGenFramebuffers");

    glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glDeleteFramebuffers");

    glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glDeleteRenderbuffers");
    glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glXGetProcAddress((const GLubyte*)"glCheckFramebufferStatus");
    glFramebufferTexture2D  = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glXGetProcAddress((const GLubyte*)"glFramebufferTexture2D");
    glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glGenRenderbuffers");
    glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindRenderbuffer");
    glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)glXGetProcAddress((const GLubyte*)"glRenderbufferStorage");
    glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)glXGetProcAddress((const GLubyte*)"glFramebufferRenderbuffer");

    glDrawBuffers = (PFNGLDRAWBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glDrawBuffers");

    glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glGenBuffers");
    glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindFramebuffer");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferSubData");

    glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glCreateProgram");
    glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress((const GLubyte*)"glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress((const GLubyte*)"glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((const GLubyte*)"glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((const GLubyte*)"glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((const GLubyte*)"glGetShaderInfoLog");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress((const GLubyte*)"glGetProgramInfoLog");
    glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress((const GLubyte*)"glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress((const GLubyte*)"glGetProgramiv");
    glDetachShader = (PFNGLDETACHSHADERPROC)glXGetProcAddress((const GLubyte*)"glDetachShader");
    glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress((const GLubyte*)"glDeleteShader");
    glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glUseProgram");
    glUniform4fv = (PFNGLUNIFORM4FVPROC)glXGetProcAddress((const GLubyte*)"glUniform4fv");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((const GLubyte*)"glGetUniformLocation");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress((const GLubyte*)"glUniformMatrix4fv");
    glUniform1i = (PFNGLUNIFORM1IPROC)glXGetProcAddress((const GLubyte*)"glUniform1i");
    glUniform3fv = (PFNGLUNIFORM3FVPROC)glXGetProcAddress((const GLubyte*)"glUniform3fv");
    glUniform1f = (PFNGLUNIFORM1FPROC)glXGetProcAddress((const GLubyte*)"glUniform1f");

    glUniform3f = (PFNGLUNIFORM3FPROC)glXGetProcAddress((const GLubyte*)"glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC)glXGetProcAddress((const GLubyte*)"glUniform4f");

    glDeleteProgram  = (PFNGLDELETEPROGRAMPROC)glXGetProcAddress((const GLubyte*)"glDeleteProgram");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)glXGetProcAddress((const GLubyte*)"glGenerateMipmap");

    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((const GLubyte*)"glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((const GLubyte*)"glBindVertexArray");
    glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferData");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((const GLubyte*)"glEnableVertexAttribArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((const GLubyte*)"glVertexAttribPointer");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glXGetProcAddress((const GLubyte*)"glDeleteBuffers");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)glXGetProcAddress((const GLubyte*)"glDeleteVertexArrays");

#endif

#if defined(_WIN32)
    glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
    glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");

		glUniform1iv = (PFNGLUNIFORM1IVPROC)wglGetProcAddress("glUniform1iv");
		glUniform4iv = (PFNGLUNIFORM4IVPROC)wglGetProcAddress("glUniform4iv");
		glUniform2iv = (PFNGLUNIFORM2IVPROC)wglGetProcAddress("glUniform2iv");
		glUniform1fv = (PFNGLUNIFORM1FVPROC)wglGetProcAddress("glUniform1fv");
		glUniform2fv = (PFNGLUNIFORM2FVPROC)wglGetProcAddress("glUniform2fv");

		glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)wglGetProcAddress("glGetActiveUniform");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");
    glVertexAttribDivisor = (PFNGLVERTEXATTRIBDIVISORPROC)wglGetProcAddress("glVertexAttribDivisor");

    glDrawArraysInstanced = (PFNGLDRAWARRAYSINSTANCEDPROC)wglGetProcAddress("glDrawArraysInstanced");
    glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)wglGetProcAddress("glCompressedTexImage2D");
    glActiveTexture = (PFNGLACTIVETEXTUREPROC)wglGetProcAddress("glActiveTexture");
    glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)wglGetProcAddress("glGenFramebuffers");

    glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)wglGetProcAddress("glDeleteFramebuffers");
    glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)wglGetProcAddress("glDeleteRenderbuffers");
    glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)wglGetProcAddress("glCheckFramebufferStatus");
    glFramebufferTexture2D  = (PFNGLFRAMEBUFFERTEXTURE2DPROC)wglGetProcAddress("glFramebufferTexture2D");
    glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)wglGetProcAddress("glGenRenderbuffers");
    glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)wglGetProcAddress("glBindRenderbuffer");
    glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)wglGetProcAddress("glRenderbufferStorage");
    glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)wglGetProcAddress("glFramebufferRenderbuffer");

    glDrawBuffers = (PFNGLDRAWBUFFERSPROC)wglGetProcAddress("glDrawBuffers");

    glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
    glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)wglGetProcAddress("glBindFramebuffer");
    glBufferSubData = (PFNGLBUFFERSUBDATAPROC)wglGetProcAddress("glBufferSubData");

    glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
    glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
    glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
    glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
    glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
    glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");
    glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");
    glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
    glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
    glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
    glDetachShader = (PFNGLDETACHSHADERPROC)wglGetProcAddress("glDetachShader");
    glDeleteShader = (PFNGLDELETESHADERPROC)wglGetProcAddress("glDeleteShader");
    glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    glUniform4fv = (PFNGLUNIFORM4FVPROC)wglGetProcAddress("glUniform4fv");
    glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
    glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
    glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
    glUniform3fv = (PFNGLUNIFORM3FVPROC)wglGetProcAddress("glUniform3fv");
    glUniform1f = (PFNGLUNIFORM1FPROC)wglGetProcAddress("glUniform1f");

    glUniform3f = (PFNGLUNIFORM3FPROC)wglGetProcAddress("glUniform3f");
    glUniform4f = (PFNGLUNIFORM4FPROC)wglGetProcAddress("glUniform4f");

    glDeleteProgram  = (PFNGLDELETEPROGRAMPROC)wglGetProcAddress("glDeleteProgram");
    glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)wglGetProcAddress("glGenerateMipmap");

    glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
    glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
    glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
    glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
    glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");
    glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
    glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)wglGetProcAddress("glDeleteVertexArrays");
#endif
}


