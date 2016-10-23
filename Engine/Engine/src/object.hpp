#pragma once

#include <vector>
#include "vec.hpp"
#include "mat.hpp"
#include "shaders.hpp"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

class Object {
public:
	Object::Object(Shader *shader, vector<Vec4> vertices, vector<Vec4> colors, vector<GLubyte> indices);
	Object(Shader * s, vector<Vec4> vertices, Vec4 color, vector<GLubyte> indices);
	~Object();
	virtual void draw(Mat4);
	virtual void setColor(Vec4);

	static const int VERTICES = 0, COLORS = 1;

private:
	GLuint vaoId, vboId[2];
	Shader &shader;
	Vertex *data;
	int nIndices;
	Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

	void createBuffers(vector<Vec4>& vertices, vector<Vec4>& colors, vector<GLubyte>& indices);
};

bool isOpenGLError();
void checkOpenGLError(std::string error);
