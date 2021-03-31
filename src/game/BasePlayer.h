#ifndef BASE_PLAYER_H_
#define BASE_PLAYER_H_

#include "util/utils.h"

class BasePlayer {
public:
    virtual ~BasePlayer() {}
    virtual void hit(int damage) {}
    virtual void update() {}
    virtual void render(class Camera *camera) {}
    virtual void trace(const vec3 &rayPos, const vec3 &rayDir, float &t) {}
};

#endif
