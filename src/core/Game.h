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

    GLuint render_program;

    class Cube *cube;
    class OpenGLRenderer* renderer;
    class Shader* shader;
    class Camera* camera;
    class Player* player;

    class Texture* floorTex;
    class Texture* wallTex;
    class Texture* medKitTex;
    class Texture* gunTex;

    // DEBUG THINGS
    class AABB* aabb;
    class Line* line;

    class UI *ui;
	class Fountain *fountainEmitter;

    int mWidth, mHeight;
};


#endif // GAME_H_
