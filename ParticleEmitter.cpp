#include "ParticleEmitter.h"

#include "Actor.h"
#include "Scene.h"

ParticleEmitter::ParticleEmitter(Actor* act, const Particle& p)
{
	this->p = p;
	this->act = act;
	this->period = 1/60.0f;
	this->state = 0;
}

ParticleEmitter::ParticleEmitter(Actor* act)
{
	this->act = act;
	this->period = 1/60.0f;
	this->state = 0;
}

void ParticleEmitter::update()
{
	state += dt;
	while(state > period)
	{
		state -= period;
		p.p = act->p;
		act->sc->addParticle(p);
	}
}
