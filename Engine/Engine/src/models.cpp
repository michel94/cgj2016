#include "models.hpp"

Model::Model(vector<Vec4> vertices, vector<GLubyte> indices){
	createBuffers(vertices, indices);
}

Model::~Model() {
	glBindVertexArray(vaoId);
	glDisableVertexAttribArray(Shader::VERTICES);
	glDeleteBuffers(2, vboId);
	glDeleteVertexArrays(1, &vaoId);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Model::createBuffers(vector<Vec4>& vertices, vector<GLubyte>& indices) {
	data = new Vertex[vertices.size()];
	for (int i = 0; i < vertices.size(); i++) {
		memcpy(data[i].XYZW, vertices[i].data(), vertices[i].size());
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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLubyte), &indices[0], GL_STATIC_DRAW);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

map<string, Model*> Models::models;

Vec4 lb = Vec4(0.0f, 0.0f, 0.0f, 1.0f),
lt = Vec4(0.0f, 1.0f, 0.0f, 1.0f),
rb = Vec4(1.0f, 0.0f, 0.0f, 1.0f),
rt = Vec4(1.0f, 1.0f, 0.0f, 1.0f);

Model* Models::getTriangle() {
	if (Models::models.find("triangle") == Models::models.end())
		Models::models["triangle"] = new Model({ lb,rb,lt }, { 0, 1, 2 });
	return Models::models["triangle"];
}
Model* Models::getSquare() {
	if (Models::models.find("square") == Models::models.end())
		Models::models["square"] = new Model({ lb, rb, lt, rt }, { 1, 3, 2, 0, 1, 2 });
	return Models::models["square"];
}
Model* Models::getParallelogram() {
	if (Models::models.find("parallelogram") == Models::models.end())
		Models::models["parallelogram"] = new Model({ lb, rb, Vec4(0.5f, 0.5f, 0.0f, 1.0f), Vec4(1.5f, 0.5f, 0.0f, 1.0f) },
		{ 1, 3, 2, 0, 1, 2 });
	return Models::models["parallelogram"];
}

Model* Models::getTriangularPrism(float h) {
	vector<Vec4> topFace = { lb,rb,lt };
	vector<Vec4> vertices;
	vector<GLubyte> indices;
	
	int size = (int)topFace.size();
	for (int i = 0; i < size; i++) {
		indices.push_back(vertices.size());
		vertices.push_back(topFace[i]);
	}
	for (int i = size-1; i >= 0; i--) {
		indices.push_back(vertices.size());
		vertices.push_back(topFace[i] + Vec4(0, 0, h, 0));
	}
	for (int i = 0; i < size; i++) {
		indices.push_back(vertices.size());
		vertices.push_back(topFace[i]);
		indices.push_back(vertices.size());
		vertices.push_back( topFace[(i + 1) % size] + Vec4(0, 0, h, 0));
		indices.push_back(vertices.size());
		vertices.push_back(topFace[(i + 1) % size]);

		indices.push_back(vertices.size());
		vertices.push_back(topFace[(i + 1) % size] + Vec4(0, 0, h, 0));
		indices.push_back(vertices.size());
		vertices.push_back(topFace[i] + Vec4(0, 0, h, 0));
		indices.push_back(vertices.size());
		vertices.push_back(topFace[i]);
	}
	cout << vertices.size() << " " << indices.size() << endl;
	for (int i = 0; i < vertices.size(); i++) {
		cout << vertices[i] << "; ";
	}
	cout << endl;
	for (int i = 0; i < indices.size(); i++) {
		cout << (int)indices[i] << "; ";
	}
	cout << endl;

	return new Model(vertices, indices);

}
Model* Models::getSquarePrism(float h){
	return NULL;
}
Model* Models::getParallelogramPrism(float h) {
	return NULL;
}
