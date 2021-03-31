#include "Camera.h"
#include "core/Platform.h"

Camera::Camera()
      : aspect(0.0f), mView(), mProj(), mViewProj(), pos(0.0f), rot(0.0f), freeCam(false)
{
}

Camera::~Camera() {}

void Camera::update()
{
    if (freeCam) {
	rot.y = GInput->mouseDelta.y * 0.001f;
	rot.x = GInput->mouseDelta.x * 0.001f;
    }


    // in order to recompute model view matrix need to make identity
    mView.identity();
    mView.rotateZ(-rot.z);
    mView.rotateX(-rot.x);
    mView.rotateY(-rot.y);

    if (freeCam) {
	vec3 dir   = vec3(mView.e20, mView.e21, mView.e22);
	vec3 right = vec3(mView.e00, mView.e01, mView.e02);
	float k = deltaTime * .3f;
	// TODO:: in window is CAPITAL LETTERS i need to fix that
	if (GInput->keyStates[KEY_A])
	{
	    pos = pos - right * k;
	}
	if (GInput->keyStates[KEY_D]) pos = pos + right * k;
	if (GInput->keyStates[KEY_W]) pos = pos - dir * k;
	if (GInput->keyStates[KEY_S]) pos = pos + dir * k;
    }
    
    mView.translate(-pos);

    mProj.perspective(90.0f, aspect, 0.01f, 100.0f);

    mViewProj = mProj * mView;
}

void Camera::setPos(vec3 p)
{
    pos = p;
}
void Camera::setRot(vec3 r)
{
    rot = r;
}
