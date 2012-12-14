#include "Particles.h"

static float texcoords[4][2] = {
	{0, 0},
	{1, 0},
	{1, 1},
	{0, 1},
};

static float vtxcoords[4][3];
vec3 cameraVec;

void setCameraVec(float rotx, float roty)
{
	rotx = toRad(rotx);
	roty = toRad(roty);
	
	cameraVec.x = sin(rotx)*cos(roty);
	cameraVec.y = -sin(roty);
	cameraVec.z = -cos(rotx)*cos(roty);
	
	normalize(cameraVec);
}

Texture particleTex;
void initParticles()
{
	particleTex.loadFromFile("particle.png");
}

Particle::Particle()
{
	size = 0.2;
	v = vec3(frand(0.2), 1, frand(0.2));
}

void set(float* a, vec3 b)
{
	a[0] = b.x;
	a[1] = b.y;
	a[2] = b.z;
}

void Particle::render()
{
	vec3 dx = cross(cameraVec, vec3(0, 1, 0));
	vec3 dy = cross(dx, cameraVec);
	normalize(dx);
	normalize(dy);
	dx *= size;
	dy *= size;
	set(vtxcoords[0], p+dx+dy);
	set(vtxcoords[1], p+dx-dy);
	set(vtxcoords[2], p-dx-dy);
	set(vtxcoords[3], p-dx+dy);
	
    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glColor3f(1, 1, 1);
	particleTex.bind();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, vtxcoords);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Particle::update(float dt)
{
	v += a*dt;
	p += v*dt;
}

float Particle::getZOrder() const 
{
	return dot(p, cameraVec);
}
