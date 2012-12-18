#ifndef _MINECRAFTSCENE_H
#define _MINECRAFTSCENE_H

#include "Scene.h"

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

#define SIZE 16

class MinecraftScene : public Scene
{
	public:

	char world[SIZE][SIZE][SIZE];
	FloatArray vertices;
	FloatArray normals;
	FloatArray texcoords;
	int quadCount;
	GLuint vboId;
	
	Texture* crateTex;

	MinecraftScene();

	char getBlock(int x, int y, int z);
	bool worldCollides(vec3 p);
	void pushTex();
	void pushNormals(float x, float y, float z);
	void buildArrays();
	void makeVbo();
	
	virtual void render();
};

#endif

