#ifndef UI_H_
#define UI_H_

#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Sprite.h"
#include "util/utils.h"

class UI {
public:
    mat4 mViewProj;
    mat4 model;
    Texture *crosshair;
    Shader *shaderProg;
    Sprite *sprite;
    int width;
    int height;

    UI();
    UI(int w, int h);
    ~UI();


    void flush();
    void begin(float aspect);
    void end();
};

#endif // UI_H_
