#include "ParticleEmitter.h"

#include "Actor.h"
#include "Scene.h"

ParticleEmitter::ParticleEmitter(Actor* act)
{
	this->act = act;
	
	//Defaults
	this->period = 1/600.0f;
	
	life = 1;
	startSize = 0.4; endSize = 0;
	startAlpha = 1; endAlpha = 0;
	startCol = vec3(0, 0, 1);
	endCol = vec3(1, 0, 0);
	
	//Private stuff
	this->state = 0;
	this->disableTimer = -1;
	this->enabled = true;
}

void ParticleEmitter::update()
{
	if(disableTimer >= 0)
	{
		disableTimer -= dt;
		if(disableTimer < 0)
			enabled = false;
	}
	
	if(!enabled) return;
	
	state += dt;
	while(state > period)
	{
		state -= period;
		spawnParticle();
	}
}

void ParticleEmitter::spawnParticle()
{
	//TODO Advance particle by needed delta time?
	Particle pt;
	pt.p = act->p;
	pt.v = act->v*2.0f;
	pt.a = a;
	pt.life = life;
	pt.startingLife = pt.life;
	pt.startSize = startSize;
	pt.endSize = endSize;
	pt.startCol = startCol;
	pt.endCol = endCol;
	pt.startAlpha = startAlpha;
	pt.endAlpha = endAlpha;
	
	act->sc->addParticle(pt);
}
