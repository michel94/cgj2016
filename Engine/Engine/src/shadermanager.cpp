#include "shaders.hpp"

const string SHADERS_PATH = "res/shaders/";

ShaderManager::~ShaderManager() {
	destroyShaders();
}

Shader* ShaderManager::getShader(string name) {
	if (shaders.find(name) != shaders.end())
		return shaders[name];
	else {
		Shader* shader = loadShader(SHADERS_PATH + name);
		mShadersLoaded &= shader->loaded;
		return (shaders[name] = shader);
	}
}
Shader* ShaderManager::getDefaultShader() {
	return getShader("colored");
}

void ShaderManager::destroyShaders() {
	for (auto s : shaders)
		delete s.second;
}

bool ShaderManager::shadersLoaded() {
	return mShadersLoaded;
}

GLuint ShaderManager::getBlockBindingId(Shader* shader, string name) {
	int nBlocks = (int)blocks.size();
	if (blocks.find(name) == blocks.end()) {
		blocks[name] = nBlocks;
		
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		
		GLuint blockId = glGetUniformBlockIndex(shader->programId, name.c_str());
		GLint blockSize;
		glGetActiveUniformBlockiv(shader->programId, blockId, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);	// get block sizes
		blockData.push_back(new GLubyte(blockSize));													// alloc block data
		glBufferData(GL_UNIFORM_BUFFER, blockSize, blockData[nBlocks], GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, nBlocks, buffer);
	}
	
	GLuint blockId = glGetUniformBlockIndex(shader->programId, name.c_str());
	glUniformBlockBinding(shader->programId, blockId, blocks[name]);
	return blocks[name];
}
