#pragma once
#include "light.hpp"
class PointLight : public Light {

public:
	PointLight(Vec3 position, Vec3 color) : Light(Vec4(position.x, position.y, position.z, 1.0f), color){}
private:
};