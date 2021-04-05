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

#include "util/Stream.h"

#define min(a,b) (((a) < (b)) ? (a) : (b))

Stream stream("../data/mesh.lvl");
    int nIndices;
    int nVertices;
    enum Type : int { MESH, LAMP} type;
    struct Vertex
    {
        vec3 coord;
    };
    typedef int Index;
    unsigned int* indices = NULL;
    //Vertex* vertices = NULL;
    float *vertices = NULL;

struct Mat4
{
    float e00, e10, e20, e30,
          e01, e11, e21, e31,
          e02, e12, e22, e32,
          e03, e13, e23, e33;
};
    Mat4 matrix;

vec3 medKitPos;
mat4 medKitTranslate;
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

    
    printf("%d\n", sizeof(type));


    while (stream.pos < stream.size)
    {
        stream.read(&type, sizeof(type));
        stream.read(&matrix, sizeof(Mat4));

        if (type != MESH) continue;

        stream.read(&nIndices, sizeof(nIndices));
        indices = new unsigned[nIndices];
        stream.read(indices, nIndices * sizeof(unsigned int));

        stream.read(&nVertices, sizeof(nVertices));
        vertices = new float[nVertices * 3];
        stream.read(vertices, nVertices * sizeof(vertices) * 3);
    }
    printf("Done loading\n");
    for (int i = 0; i < nVertices; i++)
    {
        printf("%f %f %f\n", vertices[i * 3 + 0], vertices[i * 3 + 1], vertices[i * 3 + 2]);
    }


        
    ui = new UI(mWidth, mHeight);

    cube = new Cube; 
    renderer = new OpenGLRenderer;
    camera = new Camera;
    player = new Player(Player::PLAYER_1);
    camera->setAspect((float)mWidth / (float)mHeight);
    camera->freeCam = true;


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

    renderer->drawIndexedTest(vertices, nVertices, indices, nIndices);

    renderer->addShader("../data/shaders/basic_vertex.glsl",
			"../data/shaders/basic_fragment.glsl");
    
    delete [] indices;
    delete [] vertices;

    
    floorTex = new Texture("../data/textures/floor01.jpg");
    wallTex = new Texture("../data/textures/small_brick1.bmp");
    medKitTex = new Texture("../data/textures/barrelCol.png");
    gunTex = new Texture("../data/textures/simpGun_diffuse.png");


    //medKitPos = vec3(3.7472f, -6.5186f, -0.4521f);
    medKitPos = vec3(0.0f, 2.5186f, 0.0f);
    medKitTranslate.translate(medKitPos);


    //gunModel = glm::rotate(camPosition, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
    printf("%f %f %f", matrix.e03, matrix.e13, matrix.e23);





    
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


    renderer->setModelMatrix(&medKitTranslate);
    medKitTex->bind(0);
    if (!visible)
    {
	glBindVertexArray(0);
    }
    else
    {
	renderer->batch[3]->draw_mesh();
    }
    renderer->batch[5]->draw_mesh();
    
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
    tr.translate(vec3(3.0f, -2.0f, -2.0f));



    mat4 gunMV = invModelview * tr;
    renderer->setModelMatrix(&gunMV);
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
