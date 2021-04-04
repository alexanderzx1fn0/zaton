#include "core/Platform.h"
#include "Player.h"
#include "input/Input.h"
#include "util/utils.h"
#include "scene/level2.h"

#include "Weapon.h"

extern mat4 medKitTranslate;
extern vec3 medKitPos;
extern bool visible;

void swap(float& a, float& b)
{
    float temp;
    temp = a;
    a = b;
    b = temp;
    b = a;
}

bool intersect(const vec3& ro, const vec3& rd, const Sphere& s, float& t)
{
    float t0, t1;

    vec3 L = s.center - ro;
    float tca = L.dot(rd);
    float d2 = L.dot(L) - tca*tca;
    if (d2 > s.radiusSQ)
    {
        return false;
    }

    float thc = sqrt(s.radiusSQ - d2);

    t0 = tca - thc;
    t1 = tca + thc;

    if (t0 > t1) { swap(t0, t1); }
    if (t0 < 0.0f) {
        t0 = t1;
        if (t0 < 0) {return false; }
    }

    t = t0;

    return true;
}

Player::Player(PlayerType type)
    : BasePlayer(), type(type), pos(0.0f, 0.0f, 1.0f), rot(0.0f, 0.0f, 0.0f), onGround(false), velocity(0.0f)
{
        weapon = new Weapon();
        respawn();
}


Player::~Player() {
    delete weapon;
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
        //if (GInput->mouseStates[MOUSE_LEFT]) input |= FIRE_A;
        //if (GInput->mouseStates[MOUSE_RIGHT]) input |= FIRE_B;
        if (GInput->keyStates[VK_LBUTTON]) input |= FIRE_A;
        //if (GInput->mouseStates[MOUSE_RIGHT]) input |= FIRE_B;

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

        // compute ray origin and ray direction of the ray
        vec3 ro = pos + vec3(0.0f, PLAYER_HEIGHT, 0.0f);
        mat4 tempView;
        tempView.identity();
        tempView.rotateY(rot.y);
        tempView.rotateX(rot.x);
        tempView.rotateZ(rot.z);
        vec3 rd = -vec3(tempView.e02, tempView.e12, tempView.e22);

        mat4 mInv = medKitTranslate.inverseOrtho();
        vec3 rayPosLocal = mInv * vec4(ro, 1.0);
        vec3 rayDirLocal = mInv * vec4(rd, 0.0);


        // step 2: find ray sphere intersection
        float t = 0;
        if (intersect(ro, rd, Sphere(vec3(medKitTranslate.e03, medKitTranslate.e13, medKitTranslate.e23), .5f), t))
        {
            printf("Hit %f\n", t);
            visible = false;
        }



	//if (SphereSphere(Sphere(pos, .5f), Sphere(medKitPos, 1.f)))
        if (Sphere(pos, .5f).intersect(Sphere(medKitPos, 1.f)))
	{
	    //printf("Collide with medKit\n");
	    visible = false;
	}
	else
	{
	    //printf("FAILED\n");
	}

	printf("Position of the player: %f %f %f\n", pos.x, pos.y, pos.z);
	printf("MedKitPos of the player: %f %f %f\n", medKitTranslate.e03, medKitTranslate.e13, medKitTranslate.e23);

        //weapon->fire(pos + vec3(0.0f, PLAYER_HEIGHT, 0.0f), rot, (input & FIRE_A));
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
