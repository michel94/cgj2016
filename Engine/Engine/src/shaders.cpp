#include "shaders.hpp"

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

	GLuint ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);

	glBindAttribLocation(ProgramId, Shader::VERTICES, "in_Position");
	glBindAttribLocation(ProgramId, Shader::COLORS, "in_Color");
	glLinkProgram(ProgramId);

	s->UniformId = glGetUniformLocation(ProgramId, "Matrix");
	s->programId = ProgramId;
	s->vsId = VertexShaderId;
	s->fsId = FragmentShaderId;

	//checkOpenGLError("ERROR: Could not create shaders.");
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

	return s;
}

void destroyShader(Shader* s) {
	glUseProgram(0);
	glDetachShader(s->programId, s->vsId);
	glDetachShader(s->programId, s->fsId);

	glDeleteShader(s->fsId);
	glDeleteShader(s->vsId);
	glDeleteProgram(s->programId);

	//checkOpenGLError("ERROR: Could not destroy shaders.");
}
