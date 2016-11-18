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
	void bind() {
		glUseProgram(programId);
	}

private:
	map<string, GLuint> variables;
};

Shader* loadShader(string path);
void destroyShader(Shader*);

bool isOpenGLError();
void checkOpenGLError(std::string error);
bool checkGLSLError(string location, GLuint program);


class ShaderManager {
private:
	static map<string, Shader*> shaders;
	static bool mShadersLoaded;
public:
	static Shader& getShader(string name);
	static void destroyShaders();
	static bool shadersLoaded();
};
