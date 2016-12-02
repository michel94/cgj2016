#pragma once
#include "GL\glew.h"
#include "singleton.hpp"

class Texture {
public:
	Texture();
	Texture(int id);

	void bind();
	void unbind();

	GLuint id;
};

class TextureManager : public Manager<TextureManager, Texture*> {
public:
	Texture* loadTexture(string path);
	Texture* getTexture(string name);
	virtual Texture* loadItem(string path) {
		return loadTexture(path);
	}
};

