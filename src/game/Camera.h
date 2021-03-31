#ifndef CAMERA_H_
#define CAMERA_H_

#include "input/Input.h"

class Camera
{
public:

    vec3 pos;
    vec3 rot;

    mat4 mView;
    mat4 mProj;
    mat4 mViewProj;

    float aspect;
    bool freeCam;

    Camera();
    ~Camera();

    void update();

    void setAspect(const float v) { aspect = v; }
    void setPos(vec3 p);
    void setRot(vec3 r);
};

#endif // CAMERA_H_
