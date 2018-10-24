#pragma once


std::string loadFile(const char*);
void printShaderInfoLog(GLint);
void InitGLStates(void);
int LoadShader(const char*, const char*, bool, bool, bool, GLuint &, GLuint &, GLuint &);
void display(void);
void reshape(int, int);
void ExitFunction(int);

void app(void);

