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
	void draw(Shader& shader);
	
protected:
	virtual void createData(vector<Vec4>& vertices, Vec4 color);
	virtual void createBuffers(Vertex* data, int size, vector<GLubyte>& indices);

	GLuint vaoId, vboId[2];
	Vertex *data;
	int nIndices;
};
