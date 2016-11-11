#pragma once

#include <sstream>
#include "vec.hpp"
#include "mat.hpp"

using namespace std;

class Qtrn{
private:
	const float qThreshold = (float)1.0e-5;
public:
	float x, y, z, t;

	Qtrn();
	Qtrn(float, float, float, float);
	void operator=(const Qtrn& q) {
		x = q.x;
		y = q.y;
		z = q.z;
		t = q.t;
	}

	const void toAngleAxis(float& theta, Vec4& axis);
	const void clean();
	const float quadrance();
	const float norm();
	const Qtrn normalize();
	const Qtrn conjugate();
	const Qtrn inverse();
	Qtrn operator+(Qtrn& q1);
	Qtrn operator*(const float s);
	Qtrn operator*(Qtrn& q1);
	const Mat4 toMat4();
	const Qtrn lerp(Qtrn& q1, float k);
	const Qtrn slerp(Qtrn& q1, float k);
	const bool operator==(const Qtrn& q1);
	friend ostream& operator<<(ostream& ostr, const Qtrn& q);
	//const void qPrintAngleAxis(const std::string& s, const Qtrn& q);

	static Qtrn fromAngleAxis(float theta, Vec4 axis);
};
