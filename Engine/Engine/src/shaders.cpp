#include "shaders.hpp"
#include "model.hpp"

void Shader::bind() {
	glUseProgram(programId);
}

void Shader::unbind() {
	glUseProgram(0);
}

Shader* loadShader(string path) {
	Shader *s = new Shader();

	string VertexShaderCode;
	ifstream VertexShaderStream(path + "/vertex.glsl", std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}

	string FragmentShaderCode;
	ifstream FragmentShaderStream(path + "/frag.glsl", std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (getline(FragmentShaderStream, Line)) {
			FragmentShaderCode += "\n" + Line;
		}
		FragmentShaderStream.close();
	}
	
	char const* VertexSourcePointer = VertexShaderCode.c_str();
	char const* FragmentSourcePointer = FragmentShaderCode.c_str();
	
	GLuint VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexSourcePointer, 0);
	glCompileShader(VertexShaderId);

	GLuint FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentSourcePointer, 0);
	glCompileShader(FragmentShaderId);

	s->loaded = checkGLSLError(path, "vertex shader", VertexShaderId) && checkGLSLError(path, "fragment shader", FragmentShaderId);

	GLuint ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, Model::VERTICES, "inPosition");
	glBindAttribLocation(ProgramId, 1, "inColor");

	glBindAttribLocation(ProgramId, Model::TEXCOORDS, "inTexcoord");
	glBindAttribLocation(ProgramId, Model::NORMALS, "inNormal");
	glBindAttribLocation(ProgramId, Model::TANGENTS, "inTangent");
	glBindAttribLocation(ProgramId, Model::BITANGENTS, "inBitangent");

	glLinkProgram(ProgramId);

	s->loaded &= checkGLSLError(path, "linking shaders", VertexShaderId);

	s->programId = ProgramId;
	s->vsId = VertexShaderId;
	s->fsId = FragmentShaderId;

	GLint count;

	GLint size; // size of the variable
	GLenum type; // type of the variable (float, vec3 or mat4, etc)

	const GLsizei bufSize = 16; // maximum name length
	GLchar name[bufSize]; // variable name in GLSL
	GLsizei length; // name length

	glGetProgramiv(s->programId, GL_ACTIVE_ATTRIBUTES, &count);

	for (int i = 0; i < count; i++) {
		glGetActiveAttrib(s->programId, (GLuint)i, bufSize, &length, &size, &type, name);
		(*s)[string(name)] = glGetAttribLocation(s->programId, name);
	}
	
	glGetProgramiv(s->programId, GL_ACTIVE_UNIFORMS, &count);
	for (int i = 0; i < count; i++) {
		glGetActiveUniform(s->programId, (GLuint)i, bufSize, &length, &size, &type, name);
		GLuint r = glGetUniformLocation(s->programId, name);
		(*s)[string(name)] = r;
	}

	glGetProgramiv(s->programId, GL_ACTIVE_UNIFORM_BLOCKS, &count);
	cout << "Uniform blocks " << "(" << path << ")" << count << endl;
	for (int i = 0; i < count; i++) {
		int nameLength;
		glGetActiveUniformBlockiv(s->programId, (GLuint)i, GL_UNIFORM_BLOCK_NAME_LENGTH, &nameLength);
		char* blockName = new char[nameLength + 2];
		glGetActiveUniformBlockName(s->programId, i, nameLength, NULL, &blockName[0]);
		Block* block = ShaderManager::instance().bindBlock(s, blockName);
		s->blocks()[string(blockName)] = block;
	}

	s->loaded &= checkGLSLError(path, "global shader error", VertexShaderId);
	
	return s;
}

map<string, Block*>& Shader::blocks() {
	return uniformBlocks;
}

void destroyShader(Shader* s) {
	glUseProgram(0);
	glDetachShader(s->programId, s->vsId);
	glDetachShader(s->programId, s->fsId);

	glDeleteShader(s->fsId);
	glDeleteShader(s->vsId);
	glDeleteProgram(s->programId);

	checkOpenGLError("ERROR: Could not destroy shaders.");
}

bool checkGLSLError(string path, string location, GLuint program) {
	const GLsizei maxLength = 100;
	GLsizei length;
	char message[100];
	glGetShaderInfoLog(program, maxLength, &length, message);
	if (length > 0) {
		cerr << "Error while loading shader " << path << ": ";
		cerr << "On " << location << ": " << message << endl;
	}
		
	return length == 0;
}

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error) {
	if (isOpenGLError()) {
		std::cerr << error << std::endl;
	}
}
