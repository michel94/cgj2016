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
	Mat4 V;
	//V *= Mat4::translate(position);
	V *= Mat4::translate(-position);
	V *= rotation.toMat4();
	
	//V *= Mat4::translate(Vec3(0.0f, 0.0f, -dist));
	
	/*Mat4 T = V.inverse();
	position = Vec3(V[3][0], V[3][1], V[3][2]);
	cout << position << endl;*/

	return V;
}

Mat4 SphericalCamera::getProjection(){
	if (!perspective)
		return Mat4::ortho(-2, 2, -2, 2, -5, 5);
	else
		return Mat4::perspective(30, windowWidth / (float)windowHeight, 0.5f, 10.0f);
}
