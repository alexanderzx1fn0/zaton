#ifndef FOUNTAIN_EMITTER_H_
#define FOUNTAIN_EMITTER_H_

#include <vector>

#include "particle.h"
#include "util/utils.h"
#include "graphics/shader.h"
#include "graphics/texture.h"





        class FountainEmitter
        {
        public:

            FountainEmitter(vec3 pos);
            virtual ~FountainEmitter();

            virtual void update(float deltaTime);
            virtual void render();

            void createBuffer();

            void setProj(const mat4* view);
            void setView(const mat4* view);
            void setEyePos(vec3 eyePos);
            void setWindowHeight(float height)
            {
                mViewportHeight = height;
            }

            unsigned int count;
            float mTimePerParticle;
            float mViewportHeight;
			GLuint getProgram(){
				return program;
			}
        protected:
            virtual void initParticle(CParticle& out) = 0;
            float mTime;
        private:
            void addParticle();

            GLuint vao;
            GLuint vbo;

            enum {
                MAX_PARTICLES = 1500
            };

            enum {
                POSITION_SLOT = 0,
                VELOCITY_SLOT,
                SIZE_SLOT,
                TIME_SLOT,
                LIFE_SLOT,
                COLOR_SLOT,
                MASS_SLOT
            };

            std::vector<CParticle> mParticles;
            std::vector<CParticle*> mAliveParticles;
            std::vector<CParticle*> mDeadParticles;

            GLuint program;
            //unsigned int ptTexture;
            class Texture* ptTexture;

            float mMinEmitRate;
            float mMaxEmitRate;
            float mNumNewPartsExcess;
        protected:
            vec3 mSpawnDir1;
            vec3 mSpawnDir2;

            vec4 mColor1;
            vec4 mColor2;

            vec3 position;
        };


#endif

