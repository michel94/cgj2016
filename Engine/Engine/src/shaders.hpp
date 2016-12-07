#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "singleton.hpp"

using namespace std;

struct Shader {
public:
	GLuint vsId, fsId, programId;
	bool loaded = false;
	GLuint& operator[](string name) {
		return variables[name];
	}
	void bind();
	void unbind();
	map<string, GLuint>& blocks();

private:
	map<string, GLuint> variables;
	map<string, GLuint> uniformBlocks;
};

Shader* loadShader(string path);
void destroyShader(Shader*);

bool isOpenGLError();
void checkOpenGLError(std::string error);
bool checkGLSLError(string path, string location, GLuint program);

class ShaderManager : public Singleton<ShaderManager> {
public:
	~ShaderManager();
	Shader* getShader(string name);
	Shader* getDefaultShader();
	void destroyShaders();
	bool shadersLoaded();
	GLuint getBlockBindingId(Shader* shader, string name);
private:
	map<string, Shader*> shaders;
	bool mShadersLoaded = true;
	map<string, GLuint> blocks;
	vector<GLubyte*> blockData;
};
