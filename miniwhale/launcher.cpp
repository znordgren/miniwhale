///////////////////////////////////////////////////////////////////////////////
//
// Name: Zachary Nordgren
// Created: 10/9/2018
// Version: 0.1
// 
// Description: This file launches the application by creating the Glut window
//               and setting up test and debug values.
//
///////////////////////////////////////////////////////////////////////////////

#include "includes.h"

#define _DEBUG_ true
#define _OVERLAY_ true

#define GLEW_STATIC

#define _INIT_HEIGHT_ 500
#define _INIT_WIDTH_ 700

void mydisplay(void)
{

}

int main(int argc, char** argv)
{
	// Init Window
	std::cout << "started" << std::endl;
	glutInit(&argc, argv);
	window::make_window(_INIT_HEIGHT_, _INIT_WIDTH_, 0);
	
	std::cout << "made window" << std::endl;

	// Init GLEW
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Failed to initialize GLEW." << std::endl;
		exit(-1);
	}

	if (!glewIsSupported("GL_VERSION_4_0") && !glewIsSupported("GL_ARB_vertex_buffer_object"))
	{
		std::cerr << "ARB_vertex_buffer_object not supported!" << std::endl;
		exit(-2);
	}

	std::cout << "started glew" << std::endl;

	glutDisplayFunc(mydisplay);
	glutMainLoop();

	

	// TODO: make splash screen
	// TODO: make keyboard listener
	// TODO: make mouse listener
	// TODO: make debug overlay
	// TODO: create managers
	// TODO: load all assets
	// TODO: launch app
}











