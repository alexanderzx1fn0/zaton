#include "Weapon.h"
#include <stdio.h>

Weapon::Weapon() : bulletPos(0.0f) {

}

Weapon::~Weapon() {

}

bool Weapon::trace(const vec3 &a, const vec3 &b, vec3 &pos, int damage)
{
    // intersect(r
    return true;
}

#define PISTOL_BULLET_SPEED  50.0f
void Weapon::fire(const vec3& pos, const vec3& rot, bool hold)
{

    state = hold ? STATE_FIRE : STATE_NONE;

    if (state  == STATE_NONE) { return; }

   
        mat4 mView; mView.identity();
        mView.rotateY(rot.y);
        mView.rotateX(rot.x);
        mView.rotateZ(rot.z);
        vec3 dir = -vec3(mView.e02, mView.e12, mView.e22);

        vec3 velocity = dir * PISTOL_BULLET_SPEED;

        bulletPos = pos;

        vec3 nextPos = bulletPos + velocity * deltaTime;

        if (trace(pos, nextPos, bulletPos, 10)) {
            printf("Fire\n"); 
        }
}
