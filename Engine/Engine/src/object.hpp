#pragma once

#include <vector>
#include "mat.hpp"
#include "models.hpp"
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

class Object {
public:
	Object(Shader * s, Model* m);
	~Object();
	virtual void draw(Mat4);
	void setModelMatrix(Mat4& m);

protected:
	Shader &shader;
	Model &model;
	Mat4 mat;
	
};
