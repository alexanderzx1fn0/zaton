#include "UI.h"

UI::UI()
    : width(0), height(0)
{}

UI::UI(int w, int h)
    : width(w), height(h)
{

    crosshair = new Texture("../data/textures/cross.tga");     
    shaderProg = new Shader("../data/shaders/ui_vertex.glsl", "../data/shaders/ui_fragment.glsl");

    sprite = new Sprite();

    // TODO:: I need implement a way to get resolution of the window
    vec3 pos = vec3(width / 2.0f, height / 2.0f, 0.0f);
    vec3 sc = vec3(32.0f);

    model.translate(pos);
    model.scale(sc);
}

UI::~UI()
{
    delete shaderProg;
    delete sprite;
    delete crosshair; 
}


void UI::flush() {

    shaderProg->bind();
    glUniformMatrix4fv(glGetUniformLocation(shaderProg->getID(), "viewProj"), 1, GL_FALSE, (GLfloat*)&mViewProj);
    glUniformMatrix4fv(glGetUniformLocation(shaderProg->getID(), "model"), 1, GL_FALSE, (GLfloat*)&model);
    glUniform1i(glGetUniformLocation(shaderProg->getID(), "image"), 0);

    // bind texture
    crosshair->bind(0);

    //draw sprite
    sprite->draw();

    //shaderProg->unBind();

}

void UI::begin(float aspect)
{
    glDisable(GL_DEPTH_TEST);
    mViewProj.ortho(0, (float)height * aspect, (float)height, 0, 0, 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void UI::end() {
    flush();
}
