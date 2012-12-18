#include "MinecraftScene.h"
#include "FireActor.h"

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


MinecraftScene::MinecraftScene()
{
	for(int y = 0; y < SIZE; y++)
		for(int x = 0; x < SIZE; x++)
			for(int z = 0; z < SIZE; z++)
				world[x][y][z] = y == 0;
				//world[x][y][z] = (rand() % 10) == 0;
	
	quadCount = 0;
	buildArrays();
	cout<<"QuadCount "<<quadCount<<endl;
	makeVbo();
	
	crateTex = loadTexture("crate.jpg");
	actors.push_back(new FireActor(this));
}


char MinecraftScene::getBlock(int x, int y, int z)
{
	if(x < 0 || x >= SIZE) return 0;
	if(y < 0 || y >= SIZE) return 0;
	if(z < 0 || z >= SIZE) return 0;
	return world[x][y][z];
}

bool MinecraftScene::worldCollides(vec3 p)
{
	return getBlock((int)p.x, (int)p.y, (int)p.z) != 0;
}

void MinecraftScene::pushTex()
{
	texcoords.push2(0, 0);
	texcoords.push2(0, 1);
	texcoords.push2(1, 1);
	texcoords.push2(1, 0);
}

void MinecraftScene::pushNormals(float x, float y, float z)
{
	normals.push3(x, y, z);
	normals.push3(x, y, z);
	normals.push3(x, y, z);
	normals.push3(x, y, z);
}

void MinecraftScene::buildArrays()
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

void MinecraftScene::makeVbo()
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

void MinecraftScene::render()
{
    Scene::render();

	crateTex->bind();

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
}
