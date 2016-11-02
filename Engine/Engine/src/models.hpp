#pragma once

#include <GL/glew.h>
#include <vector>
#include "vec.hpp"
#include "shaders.hpp"

using namespace std;

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

class Model {
public:
	Model(vector<Vec4> vertices, vector<GLubyte> indices, Vec4 color);
	Model() {

	}
	~Model();
	void bind() {
		glBindVertexArray(vaoId);
	}
	void draw() {
		glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_BYTE, (GLvoid*)0);
	}

protected:
	virtual void createData(vector<Vec4>& vertices, Vec4 color);
	virtual void createBuffers(Vertex* data, int size, vector<GLubyte>& indices);

	GLuint vaoId, vboId[2];
	Vertex *data;
	int nIndices;
};

template<typename M = Model>
class Models {
public:
	static Model* getTriangle(Vec4 color);
	static Model* getSquare(Vec4 color);
	static Model* getParallelogram(Vec4 color);

	static Model* getPrism(vector<Vec4>& topFace, Vec4 color);
	static Model* getTriangularPrism(Vec4 color);
	static Model* getSquarePrism(Vec4 color);
	static Model* getParallelogramPrism(Vec4 color);
private:
	static map<string, Model*> models;
};

template<typename M>
Model* Models<M>::getPrism(vector<Vec4>& topFace, Vec4 color) {
	vector<Vec4> vertices;
	vector<GLubyte> indices;
	float h = 1;

	int size = (int)topFace.size();
	for (int i = 1; i < size - 1; i++) {
		vertices.push_back(topFace[0]);
		vertices.push_back(topFace[i + 1]);
		vertices.push_back(topFace[i]);
		
	}
	for (int i = 1; i < size - 1; i++) {
		vertices.push_back(topFace[0] + Vec4(0, 0, h, 0));
		vertices.push_back(topFace[i] + Vec4(0, 0, h, 0));
		vertices.push_back(topFace[i + 1] + Vec4(0, 0, h, 0));
	}
	for (int i = 0; i < size; i++) {
		vertices.push_back(topFace[i]);
		vertices.push_back(topFace[(i + 1) % size]);
		vertices.push_back(topFace[(i + 1) % size] + Vec4(0, 0, h, 0));

		vertices.push_back(topFace[i]);
		vertices.push_back(topFace[(i + 1) % size] + Vec4(0, 0, h, 0));
		vertices.push_back(topFace[i] + Vec4(0, 0, h, 0));
		
	}
	indices.resize(vertices.size());
	for (int i = 0; i < indices.size(); i++)
		indices[i] = i;

	return new M(vertices, indices, color);
}

template<typename M>
Model* Models<M>::getTriangularPrism(Vec4 color) {
	vector<Vec4> topFace = { lb,rb,lt };
	return getPrism(topFace, color);
}
template<typename M>
Model* Models<M>::getSquarePrism(Vec4 color) {
	vector<Vec4> topFace = { lb, rb, rt, lt };
	return getPrism(topFace, color);
}
template<typename M>
Model* Models<M>::getParallelogramPrism(Vec4 color) {
	vector<Vec4> topFace = { lb, rb, Vec4(1.5f, 0.5f, 0.0f, 1.0f), Vec4(0.5f, 0.5f, 0.0f, 1.0f) };
	return getPrism(topFace, color);
}
