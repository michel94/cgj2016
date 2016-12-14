#pragma once

#include "texture.hpp"
#include "SOIL.h"
#include <iostream>

const string TEXTURE_DIR = "res/images/";

Texture::Texture() {
}
Texture::Texture(int id) {
	this->id = id;
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture* TextureManager::getTexture(string name) {
	return getItem(TEXTURE_DIR + name);
}

Texture* TextureManager::loadTexture(std::string filename) {
	GLuint id;
	glGenTextures(1, &id);
	int width, height;
	unsigned char* image =
		SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

	Texture* t = new Texture(id);

	t->bind();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	t->unbind();
	SOIL_free_image_data(image);

	return t;
}
