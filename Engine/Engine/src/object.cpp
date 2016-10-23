#include "object.hpp"

Object::Object(Shader *s, vector<Vec4> vertices, Vec4 color, vector<GLubyte> indices) : shader(*s) {
	vector<Vec4> colors;
	colors.resize(vertices.size());
	for (int i = 0; i < colors.size(); i++)
		colors[i] = color;
	createBuffers(vertices, colors, indices);
}

Object::Object(Shader *s, vector<Vec4> vertices, vector<Vec4> colors, vector<GLubyte> indices) : shader(*s) {
	createBuffers(vertices, colors, indices);
}

Object::~Object() {
	glBindVertexArray(vaoId);
	glDisableVertexAttribArray(shader["in_Position"]);
	glDisableVertexAttribArray(shader["in_Color"]);
	glDeleteBuffers(2, vboId);
	glDeleteVertexArrays(1, &vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Object::createBuffers(vector<Vec4>& vertices, vector<Vec4>& colors, vector<GLubyte>& indices) {
	data = new Vertex[vertices.size()];
	for (int i = 0; i < vertices.size(); i++) {
		memcpy(data[i].XYZW, vertices[i].data(), vertices[i].size());
		memcpy(data[i].RGBA, colors[i].data(), colors[i].size());
	}
	nIndices = (int)indices.size();
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	{
		glGenBuffers(2, vboId);

		glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), data, GL_STATIC_DRAW);
		glEnableVertexAttribArray(Shader::VERTICES);
		glVertexAttribPointer(Shader::VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(Shader::COLORS);
		glVertexAttribPointer(Shader::COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(data[0].XYZW));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLubyte), &indices[0], GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object::setColor(Vec4 color) {
	this->color = color;
}

void Object::draw(Mat4 m){
	glBindVertexArray(vaoId);
	glUseProgram(shader.programId);
	glUniformMatrix4fv(shader["Matrix"], 1, GL_TRUE, m.data);
	glUniform4fv(shader["Color"], 1, (float*)color.data() );
	
	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_BYTE, (GLvoid*)0);
	
	glUseProgram(0);
	glBindVertexArray(0);

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
		exit(EXIT_FAILURE);
	}
}