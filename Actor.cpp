#include "Actor.h"

Actor::Actor(Scene* sc)
{
	this->sc = sc;
	alive = true;
//	emitters.push_back(ParticleEmitter(this));
}

void Actor::update() {}
void Actor::render() {}

void Actor::updateEmitters()
{
	for(list<ParticleEmitter>::iterator it = emitters.begin(); it != emitters.end(); it++)
		it->update();
}
