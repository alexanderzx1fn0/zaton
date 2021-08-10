#ifndef PARTICLE_H_
#define PARTICLE_H_

#include "util/utils.h"

class CParticle
{
public:
  CParticle() { mSize = 1.0f; mLifeTime = 1.0f; mAge = 0.0f; }
  virtual ~CParticle() { }

  float mSize;
  float mLifeTime;
  float mAge;
  vec4 mColor;
  vec3 mColorStep;

  vec3 mPosition;
  vec3 mDirection;
};
    struct RealParticleFountain
    {
        vec3 initialPos;
        vec3 initialVelocity;
        float initialSize;
        float initialTime;
        float lifeTime;
		vec4 color;
    };

struct RealParticle
{
    vec3 initialPos;
    vec3 initialVelocity;
    float initialSize;
    float initialTime;
    float lifeTime;
    float mass;
    vec3 initialColor;
};

struct Particle
{
	vec3 initialPos;
	vec3 initialVelocity;
    float initialSize;
    float initialTime;
    float lifeTime;
    float mass;
    vec3 initialColor;
    bool alive;

    Particle* next;


};


#endif
