#ifndef OPENGLEXTENSIONS_H_
#define OPENGLEXTENSIONS_H_


#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <GL/wglext.h>

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNGLACTIVETEXTUREPROC  glActiveTexture;

#elif defined(__linux__)
#include <GL/glx.h>

#endif



extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLGENBUFFERSPROC     glGenBuffers;

extern PFNGLCREATEPROGRAMPROC              glCreateProgram;
extern PFNGLCREATESHADERPROC               glCreateShader;
extern PFNGLSHADERSOURCEPROC               glShaderSource;
extern PFNGLCOMPILESHADERPROC              glCompileShader;
extern PFNGLGETSHADERIVPROC                glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC           glGetShaderInfoLog;
extern PFNGLGETPROGRAMINFOLOGPROC          glGetProgramInfoLog;
extern PFNGLATTACHSHADERPROC               glAttachShader;
extern PFNGLLINKPROGRAMPROC                glLinkProgram;
extern PFNGLGETPROGRAMIVPROC               glGetProgramiv;
extern PFNGLDETACHSHADERPROC               glDetachShader;
extern PFNGLDELETESHADERPROC               glDeleteShader;
extern PFNGLUSEPROGRAMPROC                 glUseProgram;
extern PFNGLUNIFORM4FVPROC                 glUniform4fv;
extern PFNGLGETUNIFORMLOCATIONPROC         glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC           glUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC		   glUniform1i;
extern PFNGLDELETEPROGRAMPROC              glDeleteProgram;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;

/** BUFFERS **/
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLBINDBUFFERPROC     glBindBuffer;
extern PFNGLBUFFERDATAPROC     glBufferData;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC    glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC        glVertexAttribPointer;
extern PFNGLDELETEBUFFERSPROC  glDeleteBuffers;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

void init_opengl_extensions();

#endif
