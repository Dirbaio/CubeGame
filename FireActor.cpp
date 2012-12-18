#include "FireActor.h"
#include "MinecraftScene.h"

float cubeVert[24][3] = {
	{-1.0, 1.0, 1.0},{-1.0, -1.0, 1.0},{1.0, -1.0, 1.0},{1.0, 1.0, 1.0},
	{1.0, 1.0, -1.0},{1.0, -1.0, -1.0},{-1.0, -1.0, -1.0},{-1.0, 1.0, -1.0},
	{1.0, 1.0, 1.0},{1.0, -1.0, 1.0},{1.0, -1.0, -1.0},{1.0, 1.0, -1.0},
	{-1.0, 1.0, -1.0},{-1.0, -1.0, -1.0},{-1.0, -1.0, 1.0},{-1.0, 1.0, 1.0},
	{1.0, -1.0, 1.0},{-1.0, -1.0, 1.0},{-1.0, -1.0, -1.0},{1.0, -1.0, -1.0},
	{-1.0, 1.0, 1.0},{1.0, 1.0, 1.0},{1.0, 1.0, -1.0},{-1.0, 1.0, -1.0}, 
};

float cubeNormals[24][3] = {
	{0, 0, 1},{0, 0, 1},{0, 0, 1},{0, 0, 1},
	{0, 0, -1},{0, 0, -1},{0, 0, -1},{0, 0, -1},
	{1, 0, 0},{1, 0, 0},{1, 0, 0},{1, 0, 0},
	{-1, 0, 0},{-1, 0, 0},{-1, 0, 0},{-1, 0, 0},
	
	{0, -1, 0},{0, -1, 0},{0, -1, 0},{0, -1, 0},
	{0, 1, 0},{0, 1, 0},{0, 1, 0},{0, 1, 0},
};

float cubeTexcoords[24][2] = {
	{0, 0}, {0, 1}, {1, 1}, {1, 0},
	{0, 0}, {0, 1}, {1, 1}, {1, 0},
	{0, 0}, {0, 1}, {1, 1}, {1, 0},
	{0, 0}, {0, 1}, {1, 1}, {1, 0},
	{0, 0}, {0, 1}, {1, 1}, {1, 0},
	{0, 0}, {0, 1}, {1, 1}, {1, 0},
};

FireActor::FireActor(MinecraftScene* sc) : Actor(sc)
{
	this->msc = sc;

	ParticleEmitter pe (this);
	emitters.push_back(pe);
	
	playerTex = loadTexture("player.jpg");
	p = vec3(SIZE/2,SIZE+1,SIZE/2);
	size = 0.7;
}

bool FireActor::move(vec3 d)
{
	if(!collides(p+d))
	{
		p += d;
		return true;
	}
	
	float min = 0;
	float max = 1;
	
	while(max-min > 0.001)
	{
		float m = (max+min)/2;
		if(collides(p+d*m))
			max = m;
		else
			min = m;
	}
	
	p += d*min;
	return false;
}

#define VMAX 100.0f
#define ACCEL 17.0f
#define FRIC 9.0f

void FireActor::update()
{
	v.x *= exp(-dt*FRIC);
//	v.y *= exp(-dt*FRIC);
	v.z *= exp(-dt*FRIC);
	
	//TODO clamp function or whatever
	if(v.x < -VMAX) v.x = -VMAX;
	if(v.y < -VMAX) v.y = -VMAX;
	if(v.z < -VMAX) v.z = -VMAX;
	if(v.x > +VMAX) v.x = +VMAX;
	if(v.y > +VMAX) v.y = +VMAX;
	if(v.z > +VMAX) v.z = +VMAX;

	timeSinceGround += dt;

	vec3 camVec = sc->cameraLookAt - sc->cameraPos;
	camVec.y = 0;
	normalize(camVec);
	
	float dx = -camVec.z;
	float dz = -camVec.x;

	dx *= ACCEL*dt;
	dz *= ACCEL*dt;
	if(Keyboard::isKeyPressed(Keyboard::A)) v.x-=dx, v.z+=dz;
	if(Keyboard::isKeyPressed(Keyboard::D)) v.x+=dx, v.z-=dz;
	if(Keyboard::isKeyPressed(Keyboard::W)) v.x-=dz, v.z-=dx;
	if(Keyboard::isKeyPressed(Keyboard::S)) v.x+=dz, v.z+=dx;
	if(Keyboard::isKeyPressed(Keyboard::Space) && timeSinceGround < 1.0)
	{
		v.y=+12;
		timeSinceGround = 1000;
	}
	
	if(!move(vec3(v.x*dt, 0, 0))) v.x = 0;
	if(!move(vec3(0, v.y*dt, 0)))
	{
		if(v.y < 0)
			timeSinceGround = 0;
		v.y = 0;
	}
	if(!move(vec3(0, 0, v.z*dt))) v.z = 0;

	v.y -= 36*dt;
	
	float rotspeed = 3*dt;
	if(Keyboard::isKeyPressed(Keyboard::Left)) rotx -= rotspeed;
	if(Keyboard::isKeyPressed(Keyboard::Right)) rotx += rotspeed;
	if(Keyboard::isKeyPressed(Keyboard::Up)) roty -= rotspeed;
	if(Keyboard::isKeyPressed(Keyboard::Down)) roty += rotspeed;
	if(roty > 1.3) roty = 1.3;
	if(roty < -1.3) roty = -1.3;

	//CAMERA, FOLLOW ME MADAFACKA
	sc->cameraLookAt = p;
	vec3 dc (-sin(rotx), 0, cos(rotx));
	dc.x *= cos(roty);
	dc.y = sin(roty);
	dc.z *= cos(roty);
	sc->cameraPos = p+dc*6.0f;
}

bool FireActor::collides(vec3 z)
{
	//WTF
/*	if(x < 0) x--;
	if(y < 0) y--;
	if(z < 0) z--;*/

	if(msc->worldCollides(z + vec3(size/2, size/2, size/2))) return true;
	if(msc->worldCollides(z + vec3(size/2, size/2, -size/2))) return true;
	if(msc->worldCollides(z + vec3(size/2, -size/2, size/2))) return true;
	if(msc->worldCollides(z + vec3(size/2, -size/2, -size/2))) return true;
	if(msc->worldCollides(z + vec3(-size/2, size/2, size/2))) return true;
	if(msc->worldCollides(z + vec3(-size/2, size/2, -size/2))) return true;
	if(msc->worldCollides(z + vec3(-size/2, -size/2, size/2))) return true;
	if(msc->worldCollides(z + vec3(-size/2, -size/2, -size/2))) return true;
	return false;
}

void FireActor::render()
{
	glPushMatrix();
	glTranslatef(p.x, p.y, p.z);
	glScalef(0.5*size, 0.5*size, 0.5*size);
	glColor3f(1, 1, 1);
	playerTex->bind();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Set the vertex and color pointers
	glVertexPointer(3, GL_FLOAT, 0, cubeVert);
	glNormalPointer(GL_FLOAT, 0, cubeNormals);
	glTexCoordPointer(2, GL_FLOAT, 0, cubeTexcoords);

	//Draw the cube using the previously specified vertices & colors, and the specified cube indices
	glDrawArrays(GL_QUADS, 0, 24);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();
}
