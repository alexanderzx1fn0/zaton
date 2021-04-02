#include "core/Platform.h"
#include "Player.h"
#include "input/Input.h"
#include "util/utils.h"
#include "scene/level2.h"

extern vec3 medKitPos;
extern bool visible;

Player::Player(PlayerType type)
    : BasePlayer(), type(type), pos(0.0f, 0.0f, 1.0f), rot(0.0f, 0.0f, 0.0f), onGround(false), velocity(0.0f)
{
        respawn();
}


Player::~Player() {
}

void Player::respawn() {
    //Level::Object *obj = level->getRespawn(); it's maybe just Scene class
    //if (!obj) return;

    //mat4 &m = obj->matrix; // Scene class will have getModelMatrix() method
    //pos = vec3(m.e03, m.e13, m.e23); // extract from model matrix position part

    //rot = vec3(0.0f, atan2f(m.e02, -m.e22), 0.0f);
    pos = vec3(0.0f, 0.0f, 1.0f);
    rot = vec3(0.0, 0.0f, 0.0f);

    health = 100;
    velocity = vec3(0.0f);
    onGround = false;

    // TODO: need to remove it
    visible = true;

}

vec3 Player::getHeadPos() {
    return pos + vec3(0.0f, PLAYER_HEIGHT, 0.0f);
}
vec3 Player::getRot() {
    return rot;
}

// TODO: I have not corrected delta time
// and therefore weired movement system
void Player::update() {
        int32 input = 0; // reset bit mask

        // update rotation vector
        rot.y = GInput->mouseDelta.y * 0.001f;
        rot.x = GInput->mouseDelta.x * 0.001f;

        if (GInput->keyStates[KEY_R]) {
            respawn();
            GInput->keyStates[KEY_R] = false;
        }
        if (GInput->keyStates[KEY_A]) input |= LEFT;
        if (GInput->keyStates[KEY_D]) input |= RIGHT;
        if (GInput->keyStates[KEY_W]) input |= UP;
        if (GInput->keyStates[KEY_S]) input |= DOWN;
        if (GInput->keyStates[KEY_SPACE]) input |= JUMP;
        //if (GInput->keyStates[VK_LBUTTON]) input |= FIRE_A;
        //if (GInput->keyStates[VK_RBUTTON]) input |= FIRE_B;

        // i think it is direction vector

        rot.x = clamp(rot.x, -PI *0.5f, +PI * 0.5f); // limited orientation around x-axis (aka heading)

        // create rotattion matrix
        mat4 m;
        m.rotateZ(-rot.z);
        m.rotateX(-rot.x);
        m.rotateY(-rot.y);

        float fmove = 0.0f, smove = 0.0f;

        if (input & LEFT) smove = -1.0f;
        if (input & RIGHT) smove = +1.0f;
        if (input & UP)    fmove = -1.0f;
        if (input & DOWN)  fmove = +1.0f;

        // cудя по всему, тут из матрицы извлекаеи z и x оси
        vec3 dir   = vec3(m.e20, m.e21, m.e22);
        vec3 right = vec3(m.e00, m.e01, m.e02);

        velocity.x = (dir.x * fmove + right.x * smove) * PLAYER_MAX_SPEED;
        velocity.z = (dir.z * fmove + right.z * smove) * PLAYER_MAX_SPEED;

        velocity.y = velocity.y - GRAVITY * deltaTime;

        if (onGround)
            velocity.y = 0.0f;

        if ((input & JUMP) && onGround) {
            velocity.y = PLAYER_JUMP_SPEED;
        }

        pos = pos + velocity * deltaTime;

        onGround = false;

	// check collide
	collideT();
        collide();



	if (SphereSphere(Sphere(pos, .5f), Sphere(medKitPos, 1.f)))
	{
	    //printf("Collide with medKit\n");
	    visible = false;
	}
	else
	{
	    //printf("FAILED\n");
	}

	//printf("Position of the player: %f %f %f\n", pos.x, pos.y, pos.z);
	//printf("MedKitPos of the player: %f %f %f\n", medKitPos.x, medKitPos.y, medKitPos.z);
}

void Player::collide() {
    for (int i = 0; i < FloorIndicesCount; i += 3) {

	vec3 a = vec3(FloorPositions[FloorIndices[i + 0] * 3 + 0], FloorPositions[FloorIndices[i + 0] * 3 + 1], FloorPositions[FloorIndices[i + 0] * 3+2]);
	vec3 b = vec3(FloorPositions[FloorIndices[i + 1] * 3 + 0], FloorPositions[FloorIndices[i + 1] * 3 + 1], FloorPositions[FloorIndices[i + 1] * 3+2]);
	vec3 c = vec3(FloorPositions[FloorIndices[i + 2] * 3 + 0], FloorPositions[FloorIndices[i + 2] * 3 + 1], FloorPositions[FloorIndices[i + 2] * 3+2]);

	vec3 n;
	float t;

	if ((a.x == b.x && a.y == b.y && a.z == b.z) ||
	    (c.x == b.x && c.y == b.y && c.z == b.z) ||
	    (a.x == c.x && a.y == c.y && a.z == c.z)) {
	    continue;
	}

	if (Sphere(pos + vec3(0.0f, PLAYER_RADIUS, 0.0f), PLAYER_RADIUS).intersect(Triangle(a, b, c), n, t) && t != 0.0f) {
	    if (fabsf(n.y) > fabsf(n.x) && fabsf(n.y) > fabsf(n.z)) {
		onGround = true;
	    }


	    if (fabsf(n.y) < 0.1 && t > 0.0f)
	    {
		//velocity = velocity - n * velocity.dot(n);
	    }

	    pos = pos + n * t;
	}
    }
}

void Player::collideT() {
    for (int i = 0; i < WallIndicesCount; i += 3) {

	vec3 a = vec3(WallPositions[WallIndices[i + 0] * 3 + 0], WallPositions[WallIndices[i + 0] * 3 + 1], WallPositions[WallIndices[i + 0] * 3+2]);
	vec3 b = vec3(WallPositions[WallIndices[i + 1] * 3 + 0], WallPositions[WallIndices[i + 1] * 3 + 1], WallPositions[WallIndices[i + 1] * 3+2]);
	vec3 c = vec3(WallPositions[WallIndices[i + 2] * 3 + 0], WallPositions[WallIndices[i + 2] * 3 + 1], WallPositions[WallIndices[i + 2] * 3+2]);

	vec3 n;
	float t;

	if ((a.x == b.x && a.y == b.y && a.z == b.z) ||
	    (c.x == b.x && c.y == b.y && c.z == b.z) ||
	    (a.x == c.x && a.y == c.y && a.z == c.z)) {
	    continue;
	}

	if (Sphere(pos + vec3(0.0f, PLAYER_RADIUS, 0.0f), PLAYER_RADIUS).intersect(Triangle(a, b, c), n, t) && t != 0.0f) {
	    if (fabsf(n.y) > fabsf(n.x) && fabsf(n.y) > fabsf(n.z)) {
		onGround = true;
	    }


	    if (fabsf(n.y) < 0.1 && t > 0.0f)
	    {
		velocity = velocity - n * velocity.dot(n);
	    }

	    pos = pos + n * t;
	}
    }
}
