#include "vec.hpp"

Vec4::Vec4(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 1;
}
Vec4::Vec4(float x, float y, float z, float w){
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
Vec4::Vec4(const Vec3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = 1;
}
Vec4::Vec4(const Vec4 & v){
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}
void Vec4::setData(float* data) {
	memcpy(vect, data, size());
}
void* Vec4::data() const{
	return (void*) vect;
}
int Vec4::size() const{
	return 4*sizeof(float);
}
void Vec4::normalized() {
	float n = norm();
	x /= n;
	y /= n;
	z /= n;
	w /= n;
}
Vec4 Vec4::normalize() const {
	float n = norm();
	return Vec4(x/n, y/n, z/n, w/n);
}
float Vec4::norm() const{
	return sqrt(x*x + y*y + z*z + w*w);
}
float Vec4::quadrance() const{
	return x*x + y*y + z*z + w*w;
}
float Vec4::length() const {
	return norm();
}
bool Vec4::operator<(const Vec4& v) const {
	return x < v.x && y < v.y && z < v.z && w < v.w;
}
bool Vec4::operator<=(const Vec4& v) const {
	return x <= v.x && y <= v.y && z <= v.z && w <= v.w;
}
bool Vec4::operator>(const Vec4& v) const {
	return !(*this <= v);
}
bool Vec4::operator>=(const Vec4& v) const {
	return !(*this < v);
}
Vec4 Vec4::operator+(const Vec4& v) const {
	return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}
Vec4 Vec4::operator-(const Vec4& v) const {
	return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}
Vec4 Vec4::operator-() const {
	return Vec4(-x, -y, -z, -w);
}
Vec4 Vec4::operator*(float s) const {
	return Vec4(s*x, s*y, s*z, s*w);
}
Vec4 operator*(float s, Vec4& v) {
	return Vec4(s*v.x, s*v.y, s*v.z, s*v.w);
}
Vec4 Vec4::operator*(const Vec4& v) const{
	return Vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

void Vec4::operator+=(const Vec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
}
void Vec4::operator-=(const Vec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}
void Vec4::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}
void Vec4::operator*=(const Vec4& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
}
float Vec4::dot(const Vec4& v) const{
	return x * v.x + y * v.y + z * v.z + w * v.w;
}
void Vec4::operator=(const Vec4& v) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}
bool Vec4::operator==(const Vec4& v){
	return abs(x - v.x) <= 0.000001 && abs(y - v.y) <= 0.000001 && abs(z - v.z) <= 0.000001 && abs(w - v.w) <= 0.000001;
}
bool Vec4::operator!=(const Vec4& v) {
	return !(*this == v);
}
ostream& operator<<(ostream& ostr, Vec4 const& v){
	ostr << "Vec4(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
	return ostr;
}
istream& operator>>(istream& istr, Vec4 const& v) {
	istr >> v.x >> v.y >> v.z;
	return istr;
}
float Vec4::operator[](int index) {
	return vect[index];
}
