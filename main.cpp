#define GL_GLEXT_PROTOTYPES 1

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GL/glext.h"
#include <iostream>
#include <cmath>
#include <list>
#include "Particles.h"

using namespace std;
using namespace sf;


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

#define SIZE 16

char world[SIZE][SIZE][SIZE];

char getBlock(int x, int y, int z)
{
	if(x < 0 || x >= SIZE) return 0;
	if(y < 0 || y >= SIZE) return 0;
	if(z < 0 || z >= SIZE) return 0;
	return world[x][y][z];
}

class FloatArray 
{
	public:
	float* array;
	int size;
	int fill;
	
	FloatArray();
	~FloatArray();
	void push2(float x, float y);
	void push3(float x, float y, float z);
	void expand();
	void clear();
};

FloatArray::FloatArray()
{
	size = 16;
	array = new float[16];
	fill = 0;
}

void FloatArray::expand()
{
	float* newArray = new float[size*2];
	for(int i = 0; i < size; i++)
		newArray[i] = array[i];
	delete[] array;	
	array = newArray;
	size *= 2;
}

FloatArray::~FloatArray()
{
	delete[] array;
}

void FloatArray::push2(float x, float y)
{
	if(fill+2 > size)
		expand();
	array[fill++] = x;
	array[fill++] = y;
}

void FloatArray::push3(float x, float y, float z)
{
	if(fill+3 > size)
		expand();
	array[fill++] = x;
	array[fill++] = y;
	array[fill++] = z;
}

void FloatArray::clear()
{
	fill = 0;
}

FloatArray vertices;
FloatArray normals;
FloatArray texcoords;
int quadCount = 0;
GLuint vboId;

void pushTex()
{
	texcoords.push2(0, 0);
	texcoords.push2(0, 1);
	texcoords.push2(1, 1);
	texcoords.push2(1, 0);
}

void pushNormals(float x, float y, float z)
{
	normals.push3(x, y, z);
	normals.push3(x, y, z);
	normals.push3(x, y, z);
	normals.push3(x, y, z);
}

void buildArrays()
{
	for(int x = -1; x < SIZE; x++)
	for(int y = -1; y < SIZE; y++)
	for(int z = -1; z < SIZE; z++)
	{
		char b  = getBlock(x, y, z);
		char bx = getBlock(x+1, y, z);
		char by = getBlock(x, y+1, z);
		char bz = getBlock(x, y, z+1);
		
		if((b == 0) != (bx == 0))
		{
			bool n = b != 0;
			char c = n?b:bx;
			if(n)
			{
				vertices.push3(x+1, y, z);
				vertices.push3(x+1, y+1, z);
				vertices.push3(x+1, y+1, z+1);
				vertices.push3(x+1, y, z+1);
			}
			else
			{
				vertices.push3(x+1, y, z+1);
				vertices.push3(x+1, y+1, z+1);
				vertices.push3(x+1, y+1, z);
				vertices.push3(x+1, y, z);
			}
			pushTex();
			pushNormals(n?1:-1, 0, 0);
			quadCount++;
		}
		if((b == 0) != (by == 0))
		{
			bool n = b != 0;
			char c = n?b:by;
			if(!n)
			{
				vertices.push3(x, y+1, z);
				vertices.push3(x+1, y+1, z);
				vertices.push3(x+1, y+1, z+1);
				vertices.push3(x, y+1, z+1);
			}
			else
			{
				vertices.push3(x, y+1, z+1);
				vertices.push3(x+1, y+1, z+1);
				vertices.push3(x+1, y+1, z);
				vertices.push3(x, y+1, z);
			}
			pushTex();
			pushNormals(0, n?1:-1, 0);
			quadCount++;
		}
		if((b == 0) != (bz == 0))
		{
			bool n = b != 0;
			char c = n?b:bz;
			if(!n)
			{
				vertices.push3(x, y, z+1);
				vertices.push3(x, y+1, z+1);
				vertices.push3(x+1, y+1, z+1);
				vertices.push3(x+1, y, z+1);
			}
			else
			{
				vertices.push3(x+1, y, z+1);
				vertices.push3(x+1, y+1, z+1);
				vertices.push3(x, y+1, z+1);
				vertices.push3(x, y, z+1);
			}
			pushTex();
			pushNormals(0, 0, n?1:-1);
			quadCount++;
		}
	}
}

void makeVbo()
{
	uint nvertices = vertices.fill*sizeof(float);
	uint nnormals = normals.fill*sizeof(float);
	uint ntexcoords = texcoords.fill*sizeof(float);

	glGenBuffersARB(1, &vboId);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, nvertices+nnormals+ntexcoords, 0, GL_STATIC_DRAW_ARB);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, nvertices, vertices.array);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, nvertices, nnormals, normals.array);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, nvertices+nnormals, ntexcoords, texcoords.array);
}

float rotx = 0;
float roty = 0;

//Point collides with world?
bool worldCollides(float x, float y, float z)
{
	return getBlock((int)x, (int)y, (int)z) != 0;
}

class Player
{
	public:
	float x, y, z;
	float vx, vy, vz;
	float size;
	float timeSinceGround;
	
	Player();
	void update(float deltaTime);
	bool move(float dx, float dy, float dz);
	bool collides(float x, float y, float z);
};

Player::Player()
{
	x=y=z=SIZE/2;
	y = SIZE+1;
	vx=vy=vz=0;
	size = 0.7;
}

bool Player::move(float dx, float dy, float dz)
{
	if(!collides(x+dx, y+dy, z+dz))
	{
		x += dx;
		y += dy;
		z += dz;
		return true;
	}
	
	float min = 0;
	float max = 1;
	
	while(max-min > 0.001)
	{
		float m = (max+min)/2;
		if(collides(x+dx*m, y+dy*m, z+dz*m))
			max = m;
		else
			min = m;
	}
	x += dx*min;
	y += dy*min;
	z += dz*min;
	return false;
}

#define VMAX 100.0f
#define ACCEL 15.0f
#define FRIC 9.0f

void Player::update(float deltaTime)
{
	vx *= exp(-deltaTime*FRIC);
//	vy *= exp(-deltaTime*FRIC);
	vz *= exp(-deltaTime*FRIC);
	
	if(vx < -VMAX) vx = -VMAX;
	if(vy < -VMAX) vy = -VMAX;
	if(vz < -VMAX) vz = -VMAX;
	if(vx > +VMAX) vx = +VMAX;
	if(vy > +VMAX) vy = +VMAX;
	if(vz > +VMAX) vz = +VMAX;

	timeSinceGround += deltaTime;
	float rrotx = rotx/180*M_PI;
	float dx = cos(-rrotx);
	float dz = sin(-rrotx);

	dx *= ACCEL*deltaTime;
	dz *= ACCEL*deltaTime;
	if(Keyboard::isKeyPressed(Keyboard::A)) vx-=dx, vz+=dz;
	if(Keyboard::isKeyPressed(Keyboard::D)) vx+=dx, vz-=dz;
	if(Keyboard::isKeyPressed(Keyboard::W)) vx-=dz, vz-=dx;
	if(Keyboard::isKeyPressed(Keyboard::S)) vx+=dz, vz+=dx;
	if(Keyboard::isKeyPressed(Keyboard::Space) && timeSinceGround < 1.0)
	{
		vy=+10;
		timeSinceGround = 1000;
	}
	
	if(!move(vx*deltaTime, 0, 0)) vx = 0;
	if(!move(0, vy*deltaTime, 0))
	{
		if(vy < 0)
			timeSinceGround = 0;
		vy = 0;
	}
	if(!move(0, 0, vz*deltaTime)) vz = 0;

	vy -= 36*deltaTime;
}

bool Player::collides(float x, float y, float z)
{
	if(x < 0) x--;
	if(y < 0) y--;
	if(z < 0) z--;
	if(worldCollides(x+size/2, y+size/2, z+size/2)) return true;
	if(worldCollides(x+size/2, y+size/2, z-size/2)) return true;
	if(worldCollides(x+size/2, y-size/2, z+size/2)) return true;
	if(worldCollides(x+size/2, y-size/2, z-size/2)) return true;
	if(worldCollides(x-size/2, y+size/2, z+size/2)) return true;
	if(worldCollides(x-size/2, y+size/2, z-size/2)) return true;
	if(worldCollides(x-size/2, y-size/2, z+size/2)) return true;
	if(worldCollides(x-size/2, y-size/2, z-size/2)) return true;
	return false;
}

list<Particle> particles;

int main(int argc, char** argv)
{
    // Create the main window
    sf::Window app(sf::VideoMode(800, 600, 32), "SFML OpenGL");

    // Create a clock for measuring time elapsed
    sf::Clock clock;

    // Set color and depth clear value
    glClearDepth(1.f);
    glClearColor(0.f, 0.f, 0.f, 0.f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_ALPHA_TEST);
//	glAlphaFunc(GL_EQUAL, 0.0f);
	
//    glDepthMask(GL_TRUE);

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 200.0, 100.0, 300.0, 0.0 };
//	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_CULL_FACE);

	Texture crateTex, playerTex;
	crateTex.loadFromFile("crate.jpg");
	playerTex.loadFromFile("player.jpg");
	
	srand(time(NULL));
	
	for(int y = 0; y < SIZE; y++)
		for(int x = 0; x < SIZE; x++)
			for(int z = 0; z < SIZE; z++)
				world[x][y][z] = (rand() % 10) == 0;
	
	buildArrays();
	cout<<"QuadCount "<<quadCount<<endl;
	makeVbo();
	
	initParticles();
	
	Player p;
    // Start game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();

            // Escape key : exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == Keyboard::Escape))
                app.close();

            // Resize event : adjust viewport
            if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
				// Setup a perspective projection
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
			    gluPerspective(90.f, float(event.size.width)/float(event.size.height), 0.01f, 500.f);
			}
        }

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        app.setActive();

		
        // Clear color and depth buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // apply some transformations
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -SIZE/4);
        
        float deltaTime = clock.getElapsedTime().asSeconds();
        float rotspeed = 180*deltaTime;
        clock.restart();
        if(Keyboard::isKeyPressed(Keyboard::Left)) rotx -= rotspeed;
        if(Keyboard::isKeyPressed(Keyboard::Right)) rotx += rotspeed;
        if(Keyboard::isKeyPressed(Keyboard::Up)) roty -= rotspeed;
        if(Keyboard::isKeyPressed(Keyboard::Down)) roty += rotspeed;
		p.update(deltaTime);
		
		Particle pt;
		pt.p.x = p.x;
		pt.p.y = p.y;
		pt.p.z = p.z;
		particles.push_back(pt);
		cout<<particles.size()<<endl;
		
		for(list<Particle>::iterator it = particles.begin(); it  != particles.end(); it++)
			it->update(deltaTime);
		setCameraVec(rotx, roty);
		
        glRotatef(roty, 1.f, 0.f, 0.f);
        glRotatef(rotx, 0.f, 1.f, 0.f);
//		glTranslatef(-SIZE/2, -SIZE/2, -SIZE/2);
		glTranslatef(-p.x, -p.y, -p.z);


		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_CULL_FACE);
		glColor3f(1, 1, 1);


		//Bind texture
		crateTex.bind();

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glNormalPointer(GL_FLOAT, 0, (GLvoid*)(vertices.fill*sizeof(float)));
		glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)(vertices.fill*sizeof(float)+normals.fill*sizeof(float)));
		glDrawArrays(GL_QUADS, 0, quadCount*4);

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		glPushMatrix();
		glTranslatef(p.x, p.y, p.z);
		glScalef(0.5*p.size, 0.5*p.size, 0.5*p.size);
		glColor3f(1, 1, 1);
		playerTex.bind();

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



		glPushMatrix();
		glTranslatef(p.x+cameraVec.x, p.y+cameraVec.y, p.z+cameraVec.z);
		glScalef(0.5*p.size, 0.5*p.size, 0.5*p.size);
		glColor3f(1, 1, 1);
		playerTex.bind();

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


		
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
		glDisable(GL_CULL_FACE);
		
		//SORT DA PARTICLES!!!
		particles.sort();
		for(list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
			it->render();

		// Finally, display rendered frame on screen
        app.display();
    }

    return EXIT_SUCCESS;
}




