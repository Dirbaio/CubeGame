#ifndef _SCENE_H
#define _SCENE_H

#include "Particle.h"
#include<list>
#include "util.h"

using namespace std;

class Actor;

class Scene {
public:
	vec3 cameraPos;
	vec3 cameraLookAt;
	
	list<Particle> particles;
	list<Actor*> actors;
	
	Scene();
	~Scene();
	virtual void render();
	virtual void update();
	
	void addParticle(const Particle& p);
};

#endif