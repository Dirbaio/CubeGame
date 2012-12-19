#include "ParticleEmitter.h"

#include "Actor.h"
#include "Scene.h"

RandomVec::RandomVec()
{
	rad = 0;
}

RandomVec::RandomVec(float rad)
{
	this->rad = rad;
}

vec3 RandomVec::get()
{
	float angle = frand(2*M_PI);
	float z = frand(1);
	float mult = sqrt(1-z*z);
	return vec3(cos(angle)*mult*rad, sin(angle)*mult*rad, z*rad);
}

ParticleEmitter::ParticleEmitter(Actor* act)
{
	this->act = act;
	
	//Defaults
	this->period = 1/6000.0f;
	randPos.rad = 1;
	randVel.rad = 0;
	life = 1;
	randLife = 0.5;
	startSize = 0.4; endSize = 0.2;
	startAlpha = 0; endAlpha = 1;
	startCol = vec3(0, 0, 1);
	endCol = vec3(0, 0, 0);
	actorVelMult = -2;
	
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
	pt.p = act->p + randPos.get();
	pt.v = v + act->v*actorVelMult + randVel.get();
	pt.a = a;

	pt.life = life + frand(randLife);
	pt.startingLife = pt.life;
	pt.startSize = startSize;
	pt.endSize = endSize;
	pt.startCol = startCol;
	pt.endCol = endCol;
	pt.startAlpha = startAlpha;
	pt.endAlpha = endAlpha;
	
	act->sc->addParticle(pt);
}
