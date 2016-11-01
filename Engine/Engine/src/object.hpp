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
	virtual void setColor(Vec4);

protected:
	Object(Shader*);
	Shader &shader;
	Model &model;
	Vec4 color = Vec4(1.0f, 1.0f, 1.0f, 1.0f);

	
};
