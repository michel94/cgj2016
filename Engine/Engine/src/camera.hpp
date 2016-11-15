#pragma once
#include "mat.hpp"
#include "quaternion.hpp"

class Camera {
public:
	virtual Mat4 toMat4() = 0;
};

class SphericalCamera : public Camera {
public:
	SphericalCamera(int windowWidth, int windowHeight);
	virtual Mat4 toMat4();

	Qtrn rotation;
	Mat4 matRotation;
	float dist=1.5f;

	int windowWidth, windowHeight;
	bool perspective = true, useQuaternions=true;
};
