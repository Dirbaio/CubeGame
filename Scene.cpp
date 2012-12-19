#include "Scene.h"
#include "Actor.h"

Scene::Scene()
{
	actors.push_back(new Actor(this));
	cameraLookAt = vec3(0, 0, 0);
	cameraPos = vec3(10, 10, 10);
	particleTex.loadFromFile("particle.png");
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
/*	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (float)width/(float)height, zNear, zFar);
*/
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

vector<vec3> vtxArray;
vector<vec2> texArray;
vector<color> colArray;

void Scene::renderParticles()
{

	glDisable(GL_LIGHTING);
	glDisable(GL_LIGHT0);
	glDisable(GL_CULL_FACE);

	//SORT DA PARTICLES!!!
	particles.sort();
	
	//Render them!
	
	particleTex.bind();

/*	//Old way
	for(list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
		it->render();
	*/
	
	vtxArray.resize(0);
	texArray.resize(0);
	colArray.resize(0);
	
	vtxArray.reserve(particles.size()*4);
	texArray.reserve(particles.size()*4);
	colArray.reserve(particles.size()*4);
	
	for(list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
		it->renderArray(vtxArray, texArray, colArray);

    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, &vtxArray[0]);
	glTexCoordPointer(2, GL_FLOAT, 0, &texArray[0]);
	glColorPointer(4, GL_FLOAT, 0, &colArray[0]);
	
/*	cout<<vtxArray.size()<<endl;
	cout<<texArray.size()<<endl;*/
	glDrawArrays(GL_QUADS, 0, vtxArray.size());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
}

void Scene::addParticle(const Particle& p) 
{
	particles.push_back(p);
}

