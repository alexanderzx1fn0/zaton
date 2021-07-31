#ifndef FIRE_RING_H_
#define FIRE_RING_H_

#include "particleEmitter.h"

namespace game {
namespace particle {

class FireRing : public ParticleEmitter
{
public:
    FireRing();
    ~FireRing();
    void initParticle(RealParticle& out);
};

}
}

#endif
