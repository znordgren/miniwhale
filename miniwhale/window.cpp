///////////////////////////////////////////////////////////////////////////////
//
// Name: Zachary Nordgren
// Created: 10/9/2018
// Version: 0.1
// 
// Description: This file contains the window namespace and handles creation of
//               the glut window, input, and statistics.
//
///////////////////////////////////////////////////////////////////////////////

#include "includes.h"

void window::make_window(GLint height, GLint width, GLint window_number)
{
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA); // set display mode
	glutInitWindowSize(width, height);     // set window size
	glutInitWindowPosition(10, 10);
	glutCreateWindow("MiniWhale");

}

void window::bind_keyboard(void)
{
	
}

void window::bind_mouse(void)
{

}
