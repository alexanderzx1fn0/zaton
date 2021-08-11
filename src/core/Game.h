#ifndef GAME_H_
#define GAME_H_

#include "graphics/opengl/OpenGLExtensions.h"

#include <vector>


class Game
{
public:
    Game(int width, int height);

    ~Game();

    bool initGame();
    
    void render();

    void updateTick();
    void update();

    void collideWithLevel();


    GLuint render_program;

    class SkyPlane* skyPlane;
    class Terrain *mainTerrain;
    class Terrain *adjTerrain;

    class Cube *cube;
    class OpenGLRenderer* renderer;
    class Shader* shader;
    class Camera* camera;
    class Player* player;

    class Texture* floorTex;
    class Texture* wallTex;
    class Texture* medKitTex;
    class Texture* gunTex;
    class Texture* tex_wall;

    // DEBUG THINGS
    class AABB* aabb;
    class Line* line;

    class UI *ui;

    int mWidth, mHeight;
};


#endif // GAME_H_
