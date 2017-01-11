#pragma once
#include "scenenode.hpp"

class Light{
public:
	static const Vec3 WHITE, RED, GREEN, YELLOW, BLUE, BLACK;
	Vec3 position;
	Vec3 color = Light::WHITE;
	Light() {	
	}
	Light(Vec3 position) {
		this->position = position;
	}
	Light(Vec3 position, Vec3 color) {
		this->position = position;
		this->color = color;
	}
	
};
