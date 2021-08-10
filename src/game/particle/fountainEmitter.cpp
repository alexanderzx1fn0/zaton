#include "fountainEmitter.h"
#include "math/random.h"


        FountainEmitter::FountainEmitter(vec3 pos)
                : count(0)
                , mTime(0.0f)
                , mTimePerParticle(0.0025f)
                , mViewportHeight(0.0f)
                , vao(0)
                , vbo(0)
                , mSpawnDir1(-1.0f, -1.0f, -1.0f)
                , mSpawnDir2(1.0f, 1.0f, 1.0f)
                , mColor1(.1f, .1f, .1f, .1f)
                , mColor2(1.f, 1.f, 1.f, .9f)
                , mMinEmitRate(10.0f)
                , mMaxEmitRate(60.f)
                , position(pos)
                , program(0)
        {
            mParticles.resize(MAX_PARTICLES);
            mAliveParticles.resize(MAX_PARTICLES);
            mDeadParticles.resize(MAX_PARTICLES);

            for(int i = 0; i < MAX_PARTICLES; ++i)
            {
                mParticles[i].mLifeTime = -1.0f;
                mParticles[i].mAge = 0.0f;
            }


        }

        FountainEmitter::~FountainEmitter()
        {
            glDeleteProgram(program);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDeleteBuffers(1, &vbo);
            glBindVertexArray(0);
            glDeleteVertexArrays(1, &vao);
            delete ptTexture;
        }

        void FountainEmitter::addParticle()
        {
            if (!mDeadParticles.empty())
            {
                CParticle* p= mDeadParticles.back();
                initParticle(*p);

                mDeadParticles.pop_back();
                mAliveParticles.push_back(p);
            }
        }


        void FountainEmitter::createBuffer()
        {
            GLuint vs = shader::load("../data/shaders/particles/fountain/fountain_vertex.glsl", GL_VERTEX_SHADER, true);
            GLuint gs = shader::load("../data/shaders/particles/fountain/fountain_geom.glsl", GL_GEOMETRY_SHADER, true);
            GLuint fs = shader::load("../data/shaders/particles/fountain/fountain_fragment.glsl", GL_FRAGMENT_SHADER, true);

            program = glCreateProgram();
            glAttachShader(program, vs);
            glAttachShader(program, gs);
            glAttachShader(program, fs);
            glLinkProgram(program);

            glUseProgram(program);

            //ptTexture = createTexture2D(true, "../data/textures/smoke.dds");
            ptTexture = new Texture("../data/textures/particle.png");
            glUniform1i(glGetUniformLocation(program, "SpriteTex"), 0);
            glUniform1f(glGetUniformLocation(program, "gTime"), mTime);
            glUniform3f(glGetUniformLocation(program, "gAccel"), 0.0f, 0.9f, 0.0f);
            glUniform1f(glGetUniformLocation(program, "glViewportHeight"), mViewportHeight);


            GLintptr vertex_position_offset = 0;
            GLintptr vertex_velocity_offset = 3 * sizeof(float);
            GLintptr vertex_size_offset = 6 * sizeof(float);
            GLintptr vertex_time_offset = 7 * sizeof(float);
            GLintptr vertex_life_offset = 8 * sizeof(float);
            GLintptr vertex_color_offset = 9 * sizeof(float);

            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);
            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(RealParticleFountain), nullptr, GL_STREAM_DRAW);
            float *ptr = nullptr;
            glVertexAttribPointer(POSITION_SLOT, 3, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_position_offset);
            glVertexAttribPointer(VELOCITY_SLOT, 3, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_velocity_offset);
            glVertexAttribPointer(SIZE_SLOT,     1, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_size_offset);
            glVertexAttribPointer(TIME_SLOT,     1, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_time_offset);
            glVertexAttribPointer(LIFE_SLOT,     1, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_life_offset);
            glVertexAttribPointer(COLOR_SLOT,    4, GL_FLOAT, GL_FALSE, sizeof(RealParticleFountain), (GLvoid*)vertex_color_offset);
            //glVertexAttribPointer(MASS_SLOT, 1, GL_FLOAT, GL_FALSE, sizeof(RealParticle), (void*)(ptr+=1));
            glEnableVertexAttribArray( POSITION_SLOT );
            glEnableVertexAttribArray( VELOCITY_SLOT );
            glEnableVertexAttribArray( SIZE_SLOT );
            glEnableVertexAttribArray( TIME_SLOT );
            glEnableVertexAttribArray( LIFE_SLOT );
            glEnableVertexAttribArray( COLOR_SLOT );
            //glEnableVertexAttribArray( MASS_SLOT );
            glBindVertexArray(0);
        }

        void FountainEmitter::update(float deltaTime)
        {
            mTime += deltaTime;


            mDeadParticles.resize(0);
            mAliveParticles.resize(0);

            for (int i = 0; i < MAX_PARTICLES; ++i)
            {
                if ((mTime - mParticles[i].mAge) > mParticles[i].mLifeTime)
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

// create new particles
/*
            float emitRateThisFrame = math::RandomNumber(mMinEmitRate, mMaxEmitRate);
            int numNewParts = emitRateThisFrame * deltaTime;
            mNumNewPartsExcess += (float)(emitRateThisFrame * deltaTime) - numNewParts;
            if (mNumNewPartsExcess > 1.0f)
            {
                numNewParts += (int)mNumNewPartsExcess;
                mNumNewPartsExcess -= (int)mNumNewPartsExcess;
            }

            for(int i = 0; i < numNewParts; i++)
            {
                addParticle();
            }

*/
        }


        void FountainEmitter::render()
        {
             glUseProgram(program);

            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE );

            glUniform1f(glGetUniformLocation(program, "gTime"), mTime);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, ptTexture->id);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            

			if (mAliveParticles.size() > 0) {
				int numActiveParticles = 0;
				float* pData = (float*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
               // memcpy(pData, mAliveParticles.data(), mAliveParticles.size()*sizeof(RealParticleFountain));

                for (auto& particle : mAliveParticles)
                {
                    *(pData++) = particle->mPosition.x;
                    *(pData++) = particle->mPosition.y;
                    *(pData++) = particle->mPosition.y;
                    *(pData++) = particle->mDirection.x;
                    *(pData++) = particle->mDirection.y;
                    *(pData++) = particle->mDirection.z;
                    *(pData++) = particle->mSize;
                    *(pData++) = particle->mAge;
                    *(pData++) = particle->mLifeTime;
                    *(pData++) = particle->mColor.x;
                    *(pData++) = particle->mColor.y;
                    *(pData++) = particle->mColor.z;
                    *(pData++) = particle->mColor.w;
                }
				
				glUnmapBuffer(GL_ARRAY_BUFFER);


				glDrawArrays(GL_POINTS, 0, mAliveParticles.size());
				glBindVertexArray(0);

				glDisable(GL_BLEND);

			}
        }

        void FountainEmitter::setProj(const mat4* proj)
        {
			glUseProgram(program);
            glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (float*)proj);
        }
        void FountainEmitter::setView(const mat4* view)
        {
			glUseProgram(program);
            glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (float*)view);
        }

        void FountainEmitter::setEyePos(vec3 eyePos){
			glUseProgram(program);
            const float data[3] = {eyePos.x, eyePos.y, eyePos.z};
            GLint loc = glGetUniformLocation(program, "eyePos");
            if (loc == -1) {
                fprintf(stderr, "glGetUniformLocation failed, programId = %u, uniforName = %s\n", program, "eyePos");
            }

            glUniform3fv(loc, 1, (GLfloat*)data);
        }


