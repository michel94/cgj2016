#include "camera.hpp"

SphericalCamera::SphericalCamera(int windowWidth, int windowHeight) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

Mat4 SphericalCamera::toMat4() {
	Mat4 VP;
	if (!perspective)
		VP *= Mat4::ortho(-2, 2, -2, 2, -5, 5);
	else {
		VP *= Mat4::perspective(30, windowWidth / (float)windowHeight, 0.5f, 10.0f);
	}

	Mat4 V;
	V *= Mat4::translate(Vec3(0.0f, 0.0f, -dist));
	V *= rotation.toMat4();
	
	V *= Mat4::translate(Vec3(-1.0f, -1.0f, -1.0f));

	VP *= V;

	return VP;
}
