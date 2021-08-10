#ifndef WEAPON_H_
#define WEAPON_H_

#include "util/utils.h"

class Weapon
{
public:
    enum {
        STATE_NONE,
        STATE_FIRE,
    } state;

    Weapon();
    ~Weapon();
    void fire(const vec3& pos, const vec3& rot, bool hold);
    bool trace(const vec3 &a, const vec3 &b, vec3 &pos, int damage);

    vec3 bulletPos;
};

#endif
