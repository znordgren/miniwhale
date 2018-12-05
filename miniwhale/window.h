#pragma once

class Window
{
public:
	Window(int, char**);
	void make_window(GLint, GLint, GLint);
	void register_keyboard_handler(void*); // TODO: make keyboard and key classes
	void register_mouse_handler(void*); // TODO: make mouse and click classes
private:


};

