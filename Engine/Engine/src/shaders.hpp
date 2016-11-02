#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>

using namespace std;

struct Shader {
public:
	GLuint vsId, fsId, programId;
	bool loaded = false;
	GLuint& operator[](string name) {
		return variables[name];
	}

private:
	map<string, GLuint> variables;
};

Shader* loadShader(string path);
void destroyShader(Shader*);

bool isOpenGLError();
void checkOpenGLError(std::string error);
bool checkGLSLError(string location, GLuint program);
