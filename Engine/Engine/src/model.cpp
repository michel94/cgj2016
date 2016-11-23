#include "model.hpp"

const int VERTICES = 0, COLORS = 1;

Model::Model(vector<Vec4> vertices, vector<GLubyte> indices, Vec4 color){
	createData(vertices, color);
	createBuffers(data, (int)vertices.size(), indices);
}

Model::~Model() {
	glBindVertexArray(vaoId);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(2, vboId);
	glDeleteVertexArrays(1, &vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::createData(vector<Vec4>& vertices, Vec4 color) {
	data = new Vertex[vertices.size()];
	for (int i = 0; i < vertices.size(); i++) {
		memcpy(data[i].XYZW, vertices[i].data(), vertices[i].size());
		memcpy(data[i].RGBA, color.data(), color.size());
	}
}

void Model::createBuffers(Vertex* data, int size, vector<GLubyte>& indices) {
	nIndices = (int)indices.size();
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);
	{
		glGenBuffers(2, vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
		glBufferData(GL_ARRAY_BUFFER, size * sizeof(Vertex), data, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLubyte), &indices[0], GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::draw(Shader& shader) {
	glBindVertexArray(vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
	
	glEnableVertexAttribArray(COLORS);
	glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(data[0].XYZW));
	glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_BYTE, (GLvoid*)0);
}
