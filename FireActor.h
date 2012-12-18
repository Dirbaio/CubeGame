#ifndef _FIREACTOR_H
#define _FIREACTOR_H
#include "Actor.h"

class MinecraftScene;

class FireActor : public Actor
{
	public:
	
	float size;
	float timeSinceGround;
	MinecraftScene* msc;
	FireActor(MinecraftScene* sc);
	
	Texture* playerTex;
	
	float rotx, roty;
	
	virtual void render();
	virtual void update();

	bool move(vec3 d);
	bool collides(vec3 z);
};

#endif
