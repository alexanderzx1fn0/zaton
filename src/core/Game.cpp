#include <math.h>

#include "Game.h"
#include "util/utils.h"




#include "graphics/opengl/OpenGLRenderer.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/DrawDebug.h"



#include "scene/Cube.h"
#include "scene/LoadLevel.h"

#include "game/Camera.h"
#include "game/Player.h"
#include "game/Entity.h"

#include "physics/Collision.h"

#include "gui/UI.h"

#include "util/Stream.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))

vec3 medKitPos;
mat4 medKitTranslate;
bool visible = true;
mat4 gunModel;

Line line;
 vec3 roLine;
 vec3 rdLine;
bool lineUpdate = false;
float lineDistance = 0.0f;

Game::Game(int width, int height) : mWidth(width), mHeight(height)
{}

Game::~Game()
{
    ClearLevel();
    delete ui;
    delete player;
    delete camera;

    delete gunTex;

    delete renderer;
}



bool Game::initGame()
{

    
    //LoadLevel();
    LoadLevel("../data/mesh.geom");
    LoadCollidableGeometry("../data/env.lvl");
    //LoadLevel("../data/env2.lvl");
    
    printf("Load done\n");
    /*
    for (int i = 0; i < nVertices; i++)
    {
        printf("Texcoords: %f %f\n", vertices[i].texcoord.x, vertices[i].texcoord.y);
    }
    */
    printf("%d\n", entityCount);

        
    ui = new UI(mWidth, mHeight);

    renderer = new OpenGLRenderer;
    camera = new Camera;
    player = new Player(Player::PLAYER_1);
    camera->setAspect((float)mWidth / (float)mHeight);
    camera->freeCam = false;




/*
    renderer->drawIndexedModel(entities[0]->obj.vertices,
                              entities[0]->obj.nVertices,
                              entities[0]->obj.indices,
                              entities[0]->obj.nIndices);
*/

    for (int i = 0; i < entityCount; i++)
    {
        renderer->drawIndexedTest(entities[i]->obj.f_vertices,
            entities[i]->obj.nVertices,
            entities[i]->obj.indices,
            entities[i]->obj.nIndices);
    }


    renderer->addShader("../data/shaders/basic_vertex.glsl",
			"../data/shaders/basic_fragment.glsl");
    
    
    gunTex = new Texture("../data/textures/simpGun_diffuse.png");


    //medKitPos = vec3(3.7472f, -6.5186f, -0.4521f);
    medKitPos = vec3(0.0f, 2.5186f, 0.0f);
    medKitTranslate.translate(medKitPos);

    line.create();

    
    return true;
}

void Game::render() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, mWidth, mHeight);

/*
	glLineWidth(2.f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_LINE_SMOOTH);
*/



    renderer->currentShader->bind();

    
    for (int i = 0; i < entityCount-1; i++)
    {
        renderer->setModelMatrix(&entities[i]->obj.matrix);
        renderer->batch[i]->draw_mesh();
    }



    renderer->setUniform1i("colorLine", 0);
    if (lineUpdate)
    {
	renderer->setUniform1i("colorLine", 1);
	line.draw(roLine, rdLine, lineDistance);
    }
    renderer->setUniform1i("colorLine", 0);

    
    mat4 invModelview;

    invModelview.translate(player->pos + vec3(0.0, PLAYER_HEIGHT, 0.0));
    invModelview.rotateY(player->rot.y);
    invModelview.rotateX(player->rot.x);
    invModelview.rotateZ(player->rot.z);

    //mat4 e = invModelview * camera->mView;
    /*
    printf("%f %f %f %f\n %f %f %f %f\n %f %f %f %f\n %f %f %f %f\n",
	    e.e00,e.e10, e.e20,e.e30,
	    e.e01,e.e11, e.e21, e.e31,
	    e.e02,e.e12, e.e22,e.e32,
	    e.e03,e.e13, e.e23, e.e33);
    */

    mat4 tr;
    tr.scale(.05f);
    tr.rotateY(DEG2RAD*(10.0f));
    tr.translate(vec3(3.0f, -2.0f, -4.0f));

    mat4 gunMV = invModelview * tr;
    renderer->setModelMatrix(&gunMV);
    gunTex->bind(0);
    glClear(GL_DEPTH_BUFFER_BIT); // clear depth in order to weapon don't embedded in texture
    renderer->batch[entityCount-1]->draw_mesh();


    ui->begin(camera->aspect);
    ui->end();
}

void Game::updateTick()
{

    player->update();
    if (!camera->freeCam)
    {
	camera->setPos(player->getHeadPos());
	camera->setRot(player->getRot());
    }
    camera->update();
    renderer->currentShader->bind();
    renderer->setViewProjMatrix(&camera->mViewProj);

}

void Game::update()
{
    float d = deltaTime;
    while (d > 0.0f) {
	deltaTime = min(d, 1.0f / 60.0f);
	updateTick();
	d -= deltaTime;
    }
}
