#include "Scene.h"
#include "Actor.h"

Scene::Scene()
{
	actors.push_back(new Actor(this));
	cameraLookAt = vec3(0, 0, 0);
	cameraPos = vec3(10, 10, 10);
}

Scene::~Scene()
{
	for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); it++)
	{
		Actor* a = *it;
		delete a;
	}
}

void Scene::update()
{
	for(list<Particle>::iterator it = particles.begin(); it  != particles.end(); )
	{
		it->update();
		if(it->life <= 0)
		{
			list<Particle>::iterator it2 = it;
			it2++;
			particles.erase(it);
			it = it2;
		}
		else
			it++;
	}
	
	for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); )
	{
		(*it)->update();
		(*it)->updateEmitters();
		if(!(*it)->alive)
		{
			list<Actor*>::iterator it2 = it;
			it2++;
			actors.erase(it);
			it = it2;
		}
		else
			it++;
	}

	vec3 camVec = cameraLookAt - cameraPos;
	normalize(camVec);
	setCameraVec(camVec);
}

//TODO GET IT FROM DA WINDOW
int width = 800;
int height = 600;
float zNear = 0.1f;
float zFar = 1000;

void Scene::render()
{
	//TODO Put this somewhere else?
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (float)width/(float)height, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
//	glTranslatef(0, -100, 0);
	gluLookAt(  cameraPos.x, cameraPos.y, cameraPos.z, 
				cameraLookAt.x, cameraLookAt.y, cameraLookAt.z,
				0, 1, 0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);
	glColor3f(1, 1, 1);

	for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		(*it)->render();
}

void Scene::renderParticles()
{

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_CULL_FACE);

	//SORT DA PARTICLES!!!
	particles.sort();
	
	//Render them!
	for(list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
		it->render();
}

void Scene::addParticle(const Particle& p) 
{
	particles.push_back(p);
}

