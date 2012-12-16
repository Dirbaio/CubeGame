#include "Scene.h"
#include "Actor.h"

Scene::Scene()
{
	actors.push_back(new Actor(this));
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
}

void Scene::render()
{
	for(list<Actor*>::iterator it = actors.begin(); it != actors.end(); ++it)
		(*it)->render();

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

