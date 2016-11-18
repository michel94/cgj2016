#include "quaternion.hpp"

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913

Qtrn Qtrn::fromAngleAxis(float theta, Vec3 axis){
	Vec3 axisn = axis.normalize();

	Qtrn q;
	float a = theta * (float) DEGREES_TO_RADIANS;
	q.t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	q.x = axisn.x * s;
	q.y = axisn.y * s;
	q.z = axisn.z * s;
	
	q.clean();
	return q.normalize();
}

Qtrn::Qtrn(){
	t = 1;
	x = y = z = 0;
}

Qtrn::Qtrn(float t, float x, float y, float z){
	this->t = t;
	this->x = x;
	this->y = y;
	this->z = z;
}

const void Qtrn::toAngleAxis(float & theta, Vec4 & axis){
	Qtrn qn = normalize();
	theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - qn.t*qn.t);
	if (s < qThreshold) {
		axis.x = 1.0f;
		axis.y = 0.0f;
		axis.z = 0.0f;
		axis.w = 1.0f;
	}
	else {
		axis.x = qn.x / s;
		axis.y = qn.y / s;
		axis.z = qn.z / s;
		axis.w = 1.0f;
	}
}

const void Qtrn::clean(){
	if (fabs(t) < qThreshold) t = 0.0f;
	if (fabs(x) < qThreshold) x = 0.0f;
	if (fabs(y) < qThreshold) y = 0.0f;
	if (fabs(z) < qThreshold) z = 0.0f;
}

const float Qtrn::quadrance(){
	return t*t + x*x + y*y + z*z;
}

const float Qtrn::norm(){
	return sqrt(quadrance());
}

const Qtrn Qtrn::normalize(){
	float s = 1 / norm();
	return *this * s;
}

const Qtrn Qtrn::conjugate(){
	Qtrn qconj = { t, -x, -y, -z };
	return qconj;
}

const Qtrn Qtrn::inverse(){
	return *this * (conjugate(), 1.0f / quadrance());
}

Qtrn Qtrn::operator+(Qtrn & q1){
	Qtrn q;
	q.t = t + q1.t;
	q.x = x + q1.x;
	q.y = y + q1.y;
	q.z = z + q1.z;
	return q;
}

Qtrn Qtrn::operator*(const float s){
	Qtrn sq;
	sq.t = s * t;
	sq.x = s * x;
	sq.y = s * y;
	sq.z = s * z;
	return sq;
}

void Qtrn::operator*=(Qtrn & q1){
	Qtrn q;
	q.t = t * q1.t - x * q1.x - y * q1.y - z * q1.z;
	q.x = t * q1.x + x * q1.t + y * q1.z - z * q1.y;
	q.y = t * q1.y + y * q1.t + z * q1.x - x * q1.z;
	q.z = t * q1.z + z * q1.t + x * q1.y - y * q1.x;
	t = q.t;
	x = q.x;
	y = q.y;
	z = q.z;
}

Qtrn Qtrn::operator*(Qtrn & q1){
	Qtrn q;
	q.t = t * q1.t - x * q1.x - y * q1.y - z * q1.z;
	q.x = t * q1.x + x * q1.t + y * q1.z - z * q1.y;
	q.y = t * q1.y + y * q1.t + z * q1.x - x * q1.z;
	q.z = t * q1.z + z * q1.t + x * q1.y - y * q1.x;
	return q;
}

Mat4 Qtrn::toMat4(){
	Qtrn qn = normalize();

	float xx = qn.x * qn.x;
	float xy = qn.x * qn.y;
	float xz = qn.x * qn.z;
	float xt = qn.x * qn.t;
	float yy = qn.y * qn.y;
	float yz = qn.y * qn.z;
	float yt = qn.y * qn.t;
	float zz = qn.z * qn.z;
	float zt = qn.z * qn.t;

	return Mat4(
		Vec4(1.0f - 2.0f * (yy + zz), 2.0f * (xy + zt), 2.0f * (xz - yt), 0.0f),
		Vec4(2.0f * (xy - zt), 1.0f - 2.0f * (xx + zz), 2.0f * (yz + xt), 0.0f),
		Vec4(2.0f * (xz + yt), 2.0f * (yz - xt), 1.0f - 2.0f * (xx + yy), 0.0f),
		Vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

const Qtrn Qtrn::lerp(Qtrn & q1, float k){
	float cos_angle = x*q1.x + y*q1.y + z*q1.z + t*q1.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	Qtrn qi = *this * k0 + q1 * k1;
	return qi.normalize();
}

const Qtrn Qtrn::slerp(Qtrn & q1, float k){
	float angle = acos(x*q1.x + y*q1.y + z*q1.z + t*q1.t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	Qtrn qi = *this * k0 + q1 * k1;
	return qi.normalize();
}

const bool Qtrn::operator==(const Qtrn & q1){
	return (fabs(t - q1.t) < qThreshold && fabs(x - q1.x) < qThreshold &&
		fabs(y - q1.y) < qThreshold && fabs(z - q1.z) < qThreshold);
}

ostream & operator<<(ostream & ostr, const Qtrn & q)
{
	ostr << "Qtrn(" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << endl;
	return ostr;
}
