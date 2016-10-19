#include "vec.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

Vec3::Vec3(const Vec2& v){
	x = v.x;
	y = v.y;
	z = 0;
}

Vec2::Vec2(const Vec3& v) {
	x = v.x;
	y = v.y;
}

Vec2::Vec2(){
	this->x = 0;
	this->y = 0;
}
Vec2::Vec2(int x, int y) {
	this->x = (float)x;
	this->y = (float)y;
}
Vec2::Vec2(float x, float y){
	this->x = x;
	this->y = y;
}
Vec2::Vec2(double x, double y) {
	this->x = (float)x;
	this->y = (float)y;
}

Vec2::Vec2(const Vec2 & v){
	x = v.x;
	y = v.y;
}

void Vec2::setData(float* data) {
	memcpy(vect, data, size());
}
void* Vec2::data() const{
	return (void*) vect;
}
int Vec2::size() const{
	return 2*sizeof(float);
}
void Vec2::normalized() {
	float n = norm();
	x /= n;
	y /= n;
}
Vec2 Vec2::normalize() const {
	float n = norm();
	return Vec2(x/n, y/n);
}
float Vec2::norm() const{
	return sqrt(x*x + y*y);
}
float Vec2::length() const {
	return norm();
}
float Vec2::quadrance() const{
	return x*x + y*y;
}
Vec2 Vec2::operator+(const Vec2& v) const {
	return Vec2(x + v.x, y + v.y);
}
Vec2 Vec2::operator-(const Vec2& v) const {
	return Vec2(x - v.x, y - v.y);
}
Vec2 Vec2::operator-() const {
	return Vec2(-x, -y);
}
Vec2 Vec2::operator*(float s) const {
	return Vec2(s*x, s*y);
}
Vec2 Vec2::operator*(const Vec2& v) const{
	return Vec2(x * v.x, y * v.y);
}
void Vec2::operator+=(const Vec2& v) {
	x += v.x;
	y += v.y;
}
void Vec2::operator-=(const Vec2& v) {
	x -= v.x;
	y -= v.y;
}
void Vec2::operator*=(float s) {
	x *= s;
	y *= s;
}
void Vec2::operator*=(const Vec2& v) {
	x *= v.x;
	y *= v.y;
}
float Vec2::dot(const Vec2& v) const{
	return x * v.x + y * v.y;
}
void Vec2::operator=(const Vec2& v) {
	x = v.x;
	y = v.y;
}
bool Vec2::operator==(const Vec2& v){
	return abs(x - v.x) <= 0.000001 && abs(y - v.y) <= 0.000001;
}
bool Vec2::operator!=(const Vec2& v) {
	return !(*this == v);
}
ostream& operator<<(ostream& ostr, Vec2 const& v){
	ostr << "Vec2(" << v.x << ", " << v.y << ")";
	return ostr;
}
istream& operator>>(istream& istr, Vec2 const& v) {
	istr >> v.x >> v.y;
	return istr;
}
float Vec2::operator[](int index) {
	return vect[index];
}
void Vec2::test(){
	Vec2 v;
	float* d = (float*) v.data();
	v.x = 3;
	v.y = 4;
	assert(v == Vec2(3, 4));
	assert(d[0] == v.x && d[1] == v.y);

	assert(Vec2(0, 1) == Vec2(1, 1) + Vec2(-1, 0));
	assert(Vec2(0, 1) == Vec2(1, 1) - Vec2(1, 0));
	assert(Vec2(0, 1) == Vec2(1, 1) - Vec2(1, 0));
	assert(Vec2(0, 1) == Vec2(0.0f, 0.5f) * 2);
	assert(Vec2(0, 1) == 2 * Vec2(0.0f, 0.5f));
	assert(Vec2(0, 1) == Vec2(0.0, 0.5) * Vec2(2, 2));
	assert(0.0 == Vec2(0, 1).dot(Vec2(1, 0)) );
	Vec2 v2(3, 2);
	Vec2 v3(3, 2);
	v2 = ( (v2 * 1.3f) + Vec2(0.2f, 0.7f)).normalize();
	v3 *= 1.3f;
	v3 += Vec2(0.2f, 0.7f);
	v3.normalized();
	assert(v2 == v3);

	Vec2 v4;
	//istringstream is(string("0.2 3.2"));
	//is >> v4;
	//assert(v4 == Vec2(0.2f, 3.2f));
	float n = Vec2(3.45f, -2.91f).normalize().norm() - 1.0f;
	assert(n < 0.001f && n > -0.001f);

	Vec2 v6(3, 5), v5;
	v5 = v6;
	v6.x += 2;
	assert(v5 != v6);

}





