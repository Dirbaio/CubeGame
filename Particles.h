#ifndef _PARTICLES_H
#define _PARTICLES_H

#include "util.h"

void initParticles();

class Particle {
	public:
	
	vec3 p;
	vec3 v;
	vec3 a;
	float size;
	
	Particle();
	void render();
	void update(float dt);
	
	float getZOrder() const;
};

void setCameraVec(float xrot, float yrot);
inline bool operator<(const Particle& a, const Particle& b)
{
	return a.getZOrder() > b.getZOrder();
}

extern vec3 cameraVec;

#endif
