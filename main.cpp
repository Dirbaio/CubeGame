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
#include "Particle.h"
#include "Actor.h"
#include "Scene.h"
#include "MinecraftScene.h"

using namespace std;
using namespace sf;


float rotx = 0;
float roty = 0;

//Defined as extern in util.h
float dt;
float tim = 0;
Scene* sc;

int main(int argc, char** argv)
{
    // Create the main window
    sf::Window app(sf::VideoMode(800, 600, 32), "SFML OpenGL");
	app.setVerticalSyncEnabled(true);
	
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

	srand(time(NULL));
	
	initParticles();
	
	float frameTime = 0;
	int frameCount = 0;
	
	sc = new MinecraftScene();
	
    // Start game loop
    while (app.isOpen())
    {
        // Process events
        sf::Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
            {
                app.close();
                return 0;
            }

            // Escape key : exit
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == Keyboard::Escape))
            {
                app.close();
                return 0;
            }

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

        app.setActive();

		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		tim += dt;

		frameTime += dt;
		frameCount++;

		if(frameTime >= 5)
		{
			cout<<"FPS "<<frameCount/frameTime<<", Particles "<<sc->particles.size()<<endl;
			frameTime = 0;
			frameCount = 0;
		}

		sc->update();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		sc->render();
		sc->renderParticles();
		
        app.display();
    }

    return EXIT_SUCCESS;
}




