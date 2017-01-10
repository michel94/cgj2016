#pragma once
#include "light.hpp"

class DirectionalLight : public Light {
public:
	DirectionalLight(Vec3 position, Vec3 color) : Light(Vec4(position.x, position.y, position.z, 0.0f), color) {}
private:
};
