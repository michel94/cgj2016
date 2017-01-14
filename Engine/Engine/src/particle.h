#pragma once

#include "vec.hpp"

struct Particle {
	Vec3 position, speed;
	Vec4 color;
	float life, age = 0; 
	Vec2 size;

	Particle(Vec3 position, Vec3 speed, Vec4 color, float life, Vec2 size) {
		this->position = position;
		this->speed = speed;
		this->color = color;
		this->life = life;
		this->size = size;
	}
};