#ifndef OPENGLEXTENSIONS_H_
#define OPENGLEXTENSIONS_H_


#include <stdio.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <GL/gl.h>
#include <GL/glcorearb.h>
#include <GL/wglext.h>

// OpenGL
#define WGL_CONTEXT_MAJOR_VERSION_ARB     0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB     0x2092
#define WGL_CONTEXT_FLAGS_ARB             0x2094
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB  0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB      0x9126
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);

extern PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
extern PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;
extern PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
extern PFNGLACTIVETEXTUREPROC  glActiveTexture;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;


#elif defined(__linux__)
#include <GL/glx.h>
#include <GL/glu.h>

#endif


extern PFNGLMAPBUFFERPROC glMapBuffer;
extern PFNGLUNMAPBUFFERPROC glUnmapBuffer;
extern PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLVERTEXATTRIBDIVISORPROC glVertexAttribDivisor;
extern PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;
extern PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
extern PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
extern PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
extern PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
extern PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
extern PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
extern PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
extern PFNGLDRAWBUFFERSPROC glDrawBuffers;

extern PFNGLGENBUFFERSPROC     glGenBuffers;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;

extern PFNGLCREATEPROGRAMPROC              	glCreateProgram;
extern PFNGLCREATESHADERPROC               	glCreateShader;
extern PFNGLSHADERSOURCEPROC               	glShaderSource;
extern PFNGLCOMPILESHADERPROC              	glCompileShader;
extern PFNGLGETSHADERIVPROC                	glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC           	glGetShaderInfoLog;
extern PFNGLGETPROGRAMINFOLOGPROC          	glGetProgramInfoLog;
extern PFNGLATTACHSHADERPROC               	glAttachShader;
extern PFNGLLINKPROGRAMPROC                	glLinkProgram;
extern PFNGLGETPROGRAMIVPROC               	glGetProgramiv;
extern PFNGLDETACHSHADERPROC               	glDetachShader;
extern PFNGLDELETESHADERPROC               	glDeleteShader;
extern PFNGLUSEPROGRAMPROC                 	glUseProgram;
extern PFNGLUNIFORM4FVPROC                 	glUniform4fv;
extern PFNGLGETUNIFORMLOCATIONPROC         	glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC           	glUniformMatrix4fv;
extern PFNGLUNIFORM1IPROC		   						 	glUniform1i;
extern PFNGLUNIFORM3FVPROC                 	glUniform3fv;
extern PFNGLUNIFORM1FPROC                  	glUniform1f;
extern PFNGLUNIFORM3FPROC                  	glUniform3f;
extern PFNGLUNIFORM4FPROC                  	glUniform4f;
extern PFNGLUNIFORM1IVPROC 									glUniform1iv;
extern PFNGLUNIFORM4IVPROC 									glUniform4iv;
extern PFNGLUNIFORM2IVPROC 									glUniform2iv;
extern PFNGLUNIFORM1FVPROC 									glUniform1fv;
extern PFNGLUNIFORM2FVPROC 									glUniform2fv;

extern PFNGLDELETEPROGRAMPROC              	glDeleteProgram;
extern PFNGLGENERATEMIPMAPPROC							glGenerateMipmap;

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
