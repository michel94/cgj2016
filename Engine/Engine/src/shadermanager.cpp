#include "shaders.hpp"
#include <cassert>

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

Block* ShaderManager::bindBlock(Shader* shader, string name) {
	int nBlocks = (int)blocks.size();
	bool createBuffer = false;
	if (blocks.find(name) == blocks.end()) {
		blocks[name] = nBlocks;
		createBuffer = true;
		
	}
	
	GLuint blockIndex = glGetUniformBlockIndex(shader->programId, name.c_str());
	glUniformBlockBinding(shader->programId, blockIndex, blocks[name]);

	if (createBuffer) {
		Block& ub = *new Block(name);
		blockData.push_back(&ub);
		ub.bindPoint = blocks[name];

		glGetActiveUniformBlockiv(shader->programId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &ub.blockSize);	// get block sizes
		ub.data = new GLubyte[ub.blockSize];																	// alloc block data
		
		GLint nElements;
		glGetActiveUniformBlockiv(shader->programId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nElements);	// get number of uniforms
		
		GLint* indices = new GLint[nElements];
		GLint* offsets = new GLint[nElements];
		GLuint* uindices = new GLuint[nElements];

		glGetActiveUniformBlockiv(	shader->programId, 
									blockIndex, 
									GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, 
									indices); // get indices of uniforms
		
		for (int i = 0; i < nElements; i++) uindices[i] = indices[i];
		glGetActiveUniformsiv(		shader->programId, 
									nElements, 
									uindices, 
									GL_UNIFORM_OFFSET, 
									offsets); // get offsets of uniforms using their indices
		
		GLsizei length;
		const GLsizei bufSize = 16; // maximum uniform name length
		GLchar uniformName[bufSize]; // variable name in GLSL
		
		for (int i = 0; i < nElements; i++) {
			glGetActiveUniformName(shader->programId, indices[i], bufSize, &length, uniformName);
			ub.putUniform(uniformName, offsets[i]);
		}
		
		glGenBuffers(1, &ub.buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, ub.buffer);

		glBufferData(GL_UNIFORM_BUFFER, ub.blockSize, ub.data, GL_STREAM_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, blocks[name], ub.buffer);	// assign buffer to binding point

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	return blockData[blocks[name]];
}

Block* ShaderManager::getUniformBlock(string name) {
	assert(blocks.find(name) != blocks.end());
	return blockData[blocks[name]];
}

