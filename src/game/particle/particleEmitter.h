#ifndef PARTICLE_EMITTER_H_
#define PARTICLE_EMITTER_H_

#include <vector>
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "particle.h"
#include "src/renderer/opengl/openGLExtensions.h"
#include "src/renderer/texture.h"
#include "src/renderer/shader.h"



namespace game {
namespace particle {

class ParticleEmitter
{
public:
    ParticleEmitter();
    virtual ~ParticleEmitter();

    virtual void update(float deltaTime);
    virtual void render();

    void createBuffer();

    void setProj(glm::mat4& proj) const;
    void setView(glm::mat4& view) const;
    void setEyePos(glm::vec3& eyePos) const;
    void setWindowHeight(float height)
    {
        mViewportHeight = height;
    }

    unsigned int count;
    float mTimePerParticle;
    float mViewportHeight;
protected:
    virtual void initParticle(RealParticle& out) = 0;
    float mTime;
private:
    void addParticle();

    GLuint vao;
    GLuint vbo;

    enum {
        MAX_PARTICLES = 500
    };

    enum {
        POSITION_SLOT = 0,
        VELOCITY_SLOT,
        SIZE_SLOT,
        TIME_SLOT,
        LIFE_SLOT,
        MASS_SLOT,
        COLOR_SLOT
    };

    std::vector<RealParticle> mParticles;
    std::vector<RealParticle*> mAliveParticles;
    std::vector<RealParticle*> mDeadParticles;

    renderer::Shader* shaderProgram;
    unsigned int ptTexture;

    float mMinEmitRate;
    float mMaxEmitRate;
    float mNumNewPartsExcess;
};

}
}

#endif
