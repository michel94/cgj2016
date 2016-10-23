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
	GLuint vsId, fsId, programId, UniformId;
	GLuint& operator[](string name) {
		return variables[name];
	}

	static const GLuint VERTICES = 0, COLORS = 1;

private:
	map<string, GLuint> variables;
};

Shader* loadShader(string path);
void destroyShader(Shader*);

