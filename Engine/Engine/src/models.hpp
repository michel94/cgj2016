#pragma once

#include <GL/glew.h>
#include <vector>
#include "vec.hpp"
#include "shaders.hpp"

using namespace std;

typedef struct {
	GLfloat XYZW[4];
} Vertex;

class Model {
public:
	static const int VERTICES = 0;
	Model(vector<Vec4> vertices, vector<GLubyte> indices);
	~Model();
	void bind() {
		glBindVertexArray(vaoId);
	}
	void draw() {
		glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_BYTE, (GLvoid*)0);
	}

protected:
	void createBuffers(vector<Vec4>& vertices, vector<GLubyte>& indices);

	GLuint vaoId, vboId[2];
	Vertex *data;
	int nIndices;
};

class Models {
public:
	static Model* getTriangle();
	static Model* getSquare();
	static Model* getParallelogram();

	static Model* getTriangularPrism(float h);
	static Model* getSquarePrism(float h);
	static Model* getParallelogramPrism(float h);
private:
	static map<string, Model*> models;
};

