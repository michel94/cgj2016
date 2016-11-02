#include "models.hpp"

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
		glEnableVertexAttribArray(COLORS);
		glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(data[0].XYZW));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLubyte), &indices[0], GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

map<string, Model*> Models<>::models;

template<typename M>
Model* Models<M>::getTriangle(Vec4 color) {
	if (Models<>::models.find("triangle") == Models::models.end())
		Models<>::models["triangle"] = new M({ lb,rb,lt }, { 0, 1, 2 }, color);
	return Models<>::models["triangle"];

}
template<typename M>
Model* Models<M>::getSquare(Vec4 color) {
	if (Models<>::models.find("square") == Models<>::models.end())
		Models<>::models["square"] = new M({ lb, rb, lt, rt }, { 1, 3, 2, 0, 1, 2 }, color);
	return Models<>::models["square"];
}
template<typename M>
Model* Models<M>::getParallelogram(Vec4 color) {
	if (Models<>::models.find("parallelogram") == Models::models.end())
		Models<>::models["parallelogram"] = new M({ lb, rb, Vec4(0.5f, 0.5f, 0.0f, 1.0f), Vec4(1.5f, 0.5f, 0.0f, 1.0f) },
		{ 1, 3, 2, 0, 1, 2 }, color);
	return Models<>::models["parallelogram"];
}
