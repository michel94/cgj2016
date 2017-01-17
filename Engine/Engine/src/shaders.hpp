#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <cassert>

#include "singleton.hpp"
#include "mat.hpp"

using namespace std;

class Block {
public:
	Block(string name) {
		this->name = name;
	}
	Block(string name, GLuint buffer, GLubyte* data, int blockSize) {
		this->name = name;
		this->buffer = buffer;
		this->data = data;
		this->blockSize = blockSize;
	}
	void setOffset(string uniName, int offset) {
		uniforms[uniName] = offset;
	}
	int getOffset(string uniName) {
		assert(uniforms.find(uniName) != uniforms.end());
		return uniforms[uniName];
	}
	size_t nUniforms() {
		return uniforms.size();
	}
	void bind() {
		glBindBuffer(GL_UNIFORM_BUFFER,	buffer);
	}
	void putData(string name, GLubyte* data, int size) {	
		bind();
		assert(uniforms.find(name) != uniforms.end());
		glBufferSubData(GL_UNIFORM_BUFFER, uniforms[name], size, data);
	}
	void putVec(string name, Vec* v) {
		putData(name, (GLubyte*)v->data(), v->size());
	}
	void putInt(string name, int i) {
		putData(name, (GLubyte*)&i, sizeof(int));
	}
	void putFloat(string name, float f) {
		putData(name, (GLubyte*)&f, sizeof(float));
	}
	void putMat4(string name, const Mat4& m) {
		putData(name, (GLubyte*) m.data, m.byteSize());
	}

	string name;
	GLuint buffer;
	GLubyte* data;
	int blockSize;
	int bindPoint;
private:
	map<string, int> uniforms; // name -> offset
};

struct Shader {
public:
	GLuint vsId, fsId, programId;
	bool loaded = true;
	GLuint& operator[](string name) {
		return variables[name];
	}
	void bind();
	void unbind();
	map<string, Block*>& blocks();

private:
	map<string, GLuint> variables;
	map<string, Block*> uniformBlocks;
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
	Block* bindBlock(Shader* shader, string name);

	Block* getUniformBlock(string name);
private:
	map<string, Shader*> shaders;
	bool mShadersLoaded = true;
	map<string, GLuint> blocks;

	vector<Block*> blockData;
};
