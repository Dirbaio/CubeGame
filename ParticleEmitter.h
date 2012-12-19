#ifndef _PARTICLEEMITTER_H
#define _PARTICLEEMITTER_H

#include "Particle.h"

class Actor;

class RandomVec
{
	public:
	
	float rad;
	RandomVec();
	RandomVec(float rad);
	
	vec3 get();
};

class ParticleEmitter
{
	public:
	Actor* act;

	RandomVec randPos;

	vec3 v;
	float actorVelMult;
	RandomVec randVel;

	vec3 a;

	float life;
	float randLife;
		
	float startSize, endSize;
	vec3 startCol, endCol;
	float startAlpha, endAlpha;

	float period;
	ParticleEmitter(Actor* act);
	void update();

	void setOn();
	void turnOnFor(float time);

	private:
	void spawnParticle();
	
	float state;
	float disableTimer; //-1 if no timer
	bool enabled;
	
};

#endif
