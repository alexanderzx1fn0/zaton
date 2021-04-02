#include <math.h>

#include "Game.h"
#include "util/utils.h"


#include "scene/Cube.h"

#include "graphics/opengl/OpenGLRenderer.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "scene/LoadLevel.h"

#include "game/Camera.h"
#include "game/Player.h"

#include "physics/Collision.h"

#include "gui/UI.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))

vec3 medKitPos;
bool visible = true;
mat4 gunModel;

Game::Game(int width, int height) : mWidth(width), mHeight(height)
{}

Game::~Game()
{
    delete ui;
    delete player;
    delete camera;
    delete cube;

    delete floorTex;
    delete wallTex;
    delete medKitTex;
    delete gunTex;

    delete renderer;
}

bool Game::initGame()
{
    ui = new UI(mWidth, mHeight);

    cube = new Cube; 
    renderer = new OpenGLRenderer;
    camera = new Camera;
    player = new Player(Player::PLAYER_1);
    camera->setAspect((float)mWidth / (float)mHeight);
    camera->freeCam = false;


    renderer->drawIndexed(WallPositions, WallVertices,
			    WallIndices, WallIndicesCount, WallNormals, WallTexcoords);
    renderer->drawIndexed(FloorPositions, FloorVertices,
			    FloorIndices, FloorIndicesCount, FloorNormals, FloorTexcoords);
    renderer->drawIndexed(CeilingPositions, CeilingVertices,
			    CeilingIndices, CeilingIndicesCount, CeilingNormals, CeilingTexcoords);
    renderer->drawIndexed(medKitPositions, medKitVertices,
			    medKitIndices, medKitIndicesCount, medKitNormals, medKitTexcoords);
    renderer->drawIndexed(GunPositions, GunVertices,
			    GunIndices, GunIndicesCount, GunNormals, GunTexcoords);
    renderer->addShader("../data/shaders/basic_vertex.glsl",
			"../data/shaders/basic_fragment.glsl");
    

    
    floorTex = new Texture("../data/textures/floor01.jpg");
    wallTex = new Texture("../data/textures/small_brick1.bmp");
    medKitTex = new Texture("../data/textures/barrelCol.png");
    gunTex = new Texture("../data/textures/simpGun_diffuse.png");


    medKitPos = vec3(3.7472f, -6.5186f, -0.4521f);

    //gunModel = glm::rotate(camPosition, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));





    
    return true;
}

void Game::render() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, mWidth, mHeight);

    renderer->currentShader->bind();
    wallTex->bind(0);
    mat4 m;
    renderer->setModelMatrix(&m);
    renderer->batch[0]->draw_mesh();
    renderer->batch[2]->draw_mesh();
    floorTex->bind(0);
    renderer->batch[1]->draw_mesh();
    medKitTex->bind(0);
    if (!visible)
    {
	glBindVertexArray(0);
    }
    else
    {
	renderer->batch[3]->draw_mesh();
    }
    
    mat4 invModelview;

    invModelview.translate(player->pos);
    invModelview.rotateY(player->rot.y);
    invModelview.rotateX(player->rot.x);


    mat4 tr;
    tr.translate(vec3(5.0f, -5.0f, -10.0f));


    mat4 gunMV = invModelview * tr;
    //drawGun(renderer, mvp * gunMV, modelview, gunMV, camPos);
    //gunModel = camera->mView;
    //gunModel.scale(vec3(-.094));
    //gunModel = inverse(camera->mView);
    renderer->setModelMatrix(&gunMV);
    //renderer->setViewProjMatrix(&camera->mProj);
    gunTex->bind(0);
    renderer->batch[4]->draw_mesh();




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
