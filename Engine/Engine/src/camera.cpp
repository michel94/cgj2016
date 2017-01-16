#include "camera.hpp"

SphericalCamera::SphericalCamera(int windowWidth, int windowHeight) {
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;
}

Mat4 SphericalCamera::toMat4() {
	Mat4 VP;
	VP *= getProjection();
	VP *= getView();

	return VP;
}

Mat4 SphericalCamera::getView() {
	return Mat4::lookAt(position, position + rotation.toMat4() * Vec3(0, 0, 1), rotation.toMat4() * Vec3(0, 1, 0));
}

Mat4 SphericalCamera::getProjection(){
	if (!perspective)
		return Mat4::ortho(-2, 2, -2, 2, -5, 5);
	else
		return Mat4::perspective(30, windowWidth / (float)windowHeight, 0.5f, 40.0f);
}
