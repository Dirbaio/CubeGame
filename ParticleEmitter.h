#ifndef _PARTICLEEMITTER_H
#define _PARTICLEEMITTER_H

#include "Particle.h"

class Actor;

class ParticleEmitter
{
	public:
	Actor* act;

	Particle p;
	float period;
	float state;
	
	ParticleEmitter(Actor* act, const Particle& p);
	ParticleEmitter(Actor* act);
	void update();
};

#endif
