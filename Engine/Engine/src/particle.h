#pragma once

#include "vec.hpp"

struct Particle {
	Vec3 position, speed;
	Vec4 color;
	float life, age = 0;
	
	Particle(Vec3 position, Vec3 speed, Vec4 color, float life) {
		this->position = position;
		this->speed = speed;
		this->color = color;
		this->life = life;
	}
};