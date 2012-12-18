#ifndef _ACTOR_H
#define _ACTOR_H

#include "ParticleEmitter.h"
#include<list>
using namespace std;

class Scene;
class Actor
{
	public:

	bool alive;
	list<ParticleEmitter> emitters;
	Scene* sc;
	
	vec3 p, v;
	
	Actor(Scene* sc);

	virtual void render();
	virtual void update();

	void updateEmitters();
};
#endif
