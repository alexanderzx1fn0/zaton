#ifndef FOUNTAIN_H_
#define FOUNTAIN_H_

#include "fountainEmitter.h"


        class Fountain : public FountainEmitter
        {
        public:
            Fountain(vec3 pos);
			~Fountain(){};
            void initParticle(CParticle& out);
        };


#endif
