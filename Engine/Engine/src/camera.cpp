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
		VP *= Mat4::perspective(90, windowWidth / (float)windowHeight, 2.0f, 10.0f);
	}

	Mat4 V;
	V *= Mat4::translate(Vec3(0.0f, 0.0f, -dist));
	if (useQuaternions)
		V *= rotation.toMat4();
	else
		V *= matRotation;
	
	V *= Mat4::translate(Vec3(-0.5f, -0.5f, -0.4f));

	VP *= V;

	return VP;
}
