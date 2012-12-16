#include "FireActor.h"

FireActor::FireActor(Scene* sc)
{
	Particle p;
	emitters.push_back(ParticleEmitter(p));	
}

