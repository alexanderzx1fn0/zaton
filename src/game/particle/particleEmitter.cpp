#include "particleEmitter.h"
#include "src/libTexture/libTexture.h"
#include "src/math/random.h"

namespace game {
namespace particle {

ParticleEmitter::ParticleEmitter()
    : count(0)
    , mTime(0.0f)
    , mTimePerParticle(0.0025f)
    , shaderProgram(nullptr)
    , mViewportHeight(0.0f)
    , vao(0)
    , vbo(0)
{
    mParticles.resize(MAX_PARTICLES);
    mAliveParticles.resize(MAX_PARTICLES);
    mDeadParticles.resize(MAX_PARTICLES);

    for(int i = 0; i < MAX_PARTICLES; ++i)
    {
        mParticles[i].lifeTime = -1.0f;
        mParticles[i].initialTime = 0.0f;
    }


}

ParticleEmitter::~ParticleEmitter()
{
    delete shaderProgram;
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
}

void ParticleEmitter::addParticle()
{
        if (!mDeadParticles.empty())
        {
            RealParticle* p= mDeadParticles.back();
            initParticle(*p);

            mDeadParticles.pop_back();
            mAliveParticles.push_back(p);
        }
}


void ParticleEmitter::createBuffer()
{
    shaderProgram = new renderer::Shader(
                        "../data/shaders/particles/point_vertex.glsl",
                        "../data/shaders/particles/point_fragment.glsl",
                        "../data/shaders/particles/point_geom.glsl");

    shaderProgram->Bind();

    ptTexture = createTexture2D(true, "../data/textures/torch.dds");
    //renderer::Uniform<int>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "SpriteTex"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram->GetHandle(), "SpriteTex"), 0);
    //renderer::Uniform<float>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "Size2"), 0.5f);
    //renderer::Uniform<float>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "gTime"), get_running_time(timer));
    glUniform1f(glGetUniformLocation(shaderProgram->GetHandle(), "gTime"), mTime);
    //renderer::Uniform<vec3>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "gAccel"), vec3(0.0f, 0.9f, 0.0f));
    renderer::shader_uniform_3f(shaderProgram->GetHandle(), "gAccel", 0.0f, 0.9f, 0.0f);
    glUniform1f(glGetUniformLocation(shaderProgram->GetHandle(), "glViewportHeight"), mViewportHeight);




    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(RealParticle), nullptr, GL_STREAM_DRAW);
    float *ptr = nullptr;
    glVertexAttribPointer(POSITION_SLOT, 3, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)(ptr+=0));
    glVertexAttribPointer(VELOCITY_SLOT, 3, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)(ptr+=3));
    glVertexAttribPointer(SIZE_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)(ptr+=3));
    glVertexAttribPointer(TIME_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)(ptr+=1));
    glVertexAttribPointer(LIFE_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)(ptr+=1));
    glVertexAttribPointer(MASS_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)(ptr+=1));
    glVertexAttribPointer(COLOR_SLOT, 3, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)(ptr+=1));
    glEnableVertexAttribArray( POSITION_SLOT );
    glEnableVertexAttribArray( VELOCITY_SLOT );
    glEnableVertexAttribArray( SIZE_SLOT );
    glEnableVertexAttribArray( TIME_SLOT );
    glEnableVertexAttribArray( LIFE_SLOT );
    glEnableVertexAttribArray( MASS_SLOT );
    glEnableVertexAttribArray( COLOR_SLOT );
    glBindVertexArray(0);
}

void ParticleEmitter::update(float deltaTime)
{
    mTime += deltaTime;


    mDeadParticles.resize(0);
    mAliveParticles.resize(0);

    for (int i = 0; i < MAX_PARTICLES; ++i)
    {
        if ((mTime - mParticles[i].initialTime) > mParticles[i].lifeTime)
        {
            mDeadParticles.push_back(&mParticles[i]);
        }
        else
        {
            mAliveParticles.push_back(&mParticles[i]);
        }
    }


    // A negative or zero mTimePerParticle value denotes
    // not to emit any particles.
    if( mTimePerParticle > 0.0f )
    {
        // Emit particles.
        static float timeAccum = 0.0f;
        timeAccum += deltaTime;
        while( timeAccum >= mTimePerParticle )
        {
            addParticle();
            timeAccum -= mTimePerParticle;
        }
    }




}


void ParticleEmitter::render()
{
    shaderProgram->Bind();

    //glDisable(GL_DEPTH_TEST);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE );

    glUniform1f(glGetUniformLocation(shaderProgram->GetHandle(), "gTime"), mTime);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ptTexture);
    glBindVertexArray(vao);

    int numActiveParticles = 0;
    auto* pData = static_cast<float*>(glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY));
    for (auto & mAliveParticle : mAliveParticles)
    {
        *(pData++)=mAliveParticle->initialPos.x;
        *(pData++)=mAliveParticle->initialPos.y;
        *(pData++)=mAliveParticle->initialPos.z;
        *(pData++)=mAliveParticle->initialVelocity.x;
        *(pData++)=mAliveParticle->initialVelocity.y;
        *(pData++)=mAliveParticle->initialVelocity.z;
        *(pData++)=mAliveParticle->initialSize;
        *(pData++)=mAliveParticle->initialTime;
        *(pData++)=mAliveParticle->lifeTime;
        *(pData++)=mAliveParticle->mass;
        *(pData++)=mAliveParticle->initialColor.x;
        *(pData++)=mAliveParticle->initialColor.y;
        *(pData++)=mAliveParticle->initialColor.z;
        numActiveParticles++;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

    glDrawArrays(GL_POINTS, 0, numActiveParticles);
    glBindVertexArray(0);

    //glDisable(GL_BLEND);

}

void ParticleEmitter::setProj(glm::mat4& proj) const
{
    //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ProjectionMatrix"), proj);
    renderer::shader_uniform_mat4(shaderProgram->GetHandle(), "ProjectionMatrix", glm::value_ptr(proj));
}
void ParticleEmitter::setView(glm::mat4& view) const
{
    //renderer::Uniform<mat4>::Set(glGetUniformLocation(shaderProgram->GetHandle(), "ModelViewMatrix"), view);
    renderer::shader_uniform_mat4(shaderProgram->GetHandle(), "ModelViewMatrix", glm::value_ptr(view));
}

void ParticleEmitter::setEyePos(glm::vec3 &eyePos) const {
    renderer::shader_uniform_3f(shaderProgram->GetHandle(), "eyePos", eyePos.x, eyePos.y, eyePos.z);
}


}
}
