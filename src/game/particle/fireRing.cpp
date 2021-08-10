#include "fireRing.h"

#include <cmath>
#include "src/math/random.h"
#include "src/math/constant.h"
#include <glm/gtc/constants.hpp>

namespace game {
namespace particle {

FireRing::FireRing()
{}
FireRing::~FireRing() {};
void FireRing::initParticle(RealParticle& out)
{
    out.initialTime = mTime;
    out.lifeTime = math::GetRandomFloat(2.0f, 4.0f);
    out.initialSize = math::GetRandomFloat(.5f, 1.0f);
    math::GetRandomVec(out.initialVelocity);
    out.mass = math::GetRandomFloat(1.0f, 2.0f);
    out.initialColor =math::WHITE * math::GetRandomFloat(0.5f, 1.0f);

    float r = math::GetRandomFloat(10.0f, 14.0f);
    float t = math::GetRandomFloat(0, 2.0f * glm::pi<float>());

    out.initialPos.x = r* cos(t);
    out.initialPos.y = r* sin(t);
    out.initialPos.z = math::GetRandomFloat(-1.0f, 1.0f);
}


}
}

