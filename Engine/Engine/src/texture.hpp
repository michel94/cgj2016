#pragma once
#include "GL\glew.h"

class Texture {
public:
	Texture();

	void bind();
	void unbind();

	GLuint id;
};