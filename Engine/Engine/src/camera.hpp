#pragma once

#include "mat.hpp"
#include "quaternion.hpp"

#include "scene.hpp"
#include "scenenode.hpp"

class Camera : public SceneNode{
public:
	virtual Mat4 toMat4() = 0;
	virtual Mat4 getView() = 0;
	virtual Mat4 getProjection() = 0;
};

class SphericalCamera : public Camera {
public:
	SphericalCamera(int windowWidth, int windowHeight);
	virtual Mat4 toMat4();
	virtual Mat4 getView();
	virtual Mat4 getProjection();

	Qtrn rotation;
	Mat4 matRotation;
	float dist=4.0f;

	int windowWidth, windowHeight;
	bool perspective = true, useQuaternions=true;
};
