#include "vec.hpp"

Vec3::Vec3(){
	this->x = 0;
	this->y = 0;
	this->z = 0;
}
Vec3::Vec3(const Vec3& v) {
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}
Vec3::Vec3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}
void* Vec3::data() const{
	return (void*) vect;
}
void Vec3::setData(float* data) {
	memcpy(vect, data, size());
}
int Vec3::size() const{
	return 3*sizeof(float);
}
void Vec3::normalized() {
	float n = norm();
	x /= n;
	y /= n;
	z /= n;
}
Vec3 Vec3::normalize() const {
	float n = norm();
	return Vec3(x/n, y/n, z/n);
}
float Vec3::norm() const{
	return sqrt(x*x + y*y + z*z);
}
float Vec3::quadrance() const{
	return x*x + y*y + z*z;
}
float Vec3::length() const {
	return norm();
}
bool Vec3::operator<(const Vec3& v) const {
	return x < v.x && y < v.y && z < v.z;
}
bool Vec3::operator<=(const Vec3& v) const {
	return x <= v.x && y <= v.y && z <= v.z;
}
bool Vec3::operator>(const Vec3& v) const {
	return !(*this <= v);
}
bool Vec3::operator>=(const Vec3& v) const {
	return !(*this < v);
}
Vec3 Vec3::operator+(const Vec3& v) const {
	return Vec3(x + v.x, y + v.y, z + v.z);
}
Vec3 Vec3::operator-(const Vec3& v) const {
	return Vec3(x - v.x, y - v.y, z - v.z);
}
Vec3 Vec3::operator-() const {
	return Vec3(-x, -y, -z);
}
Vec3 Vec3::operator*(float s) const {
	return Vec3(s*x, s*y, s*z);
}
Vec3 operator*(float s, Vec3& v) {
	return Vec3(s*v.x, s*v.y, s*v.z);
}
Vec3 Vec3::operator*(const Vec3& v) const{
	return Vec3(x * v.x, y * v.y, z * v.z);
}

void Vec3::operator+=(const Vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
}
void Vec3::operator-=(const Vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
}
void Vec3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
}
void Vec3::operator*=(const Vec3& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
}

Vec3 Vec3::cross(const Vec3& v) const{
	return Vec3(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}
float Vec3::dot(const Vec3& v) const{
	return x * v.x + y * v.y + z * v.z;
}
void Vec3::operator=(const Vec3& v) {
	x = v.x;
	y = v.y;
	z = v.z;
}
bool Vec3::operator==(const Vec3& v){
	return abs(x - v.x) <= 0.000001 && abs(y - v.y) <= 0.000001 && abs(z - v.z) <= 0.000001;
}
bool Vec3::operator!=(const Vec3& v) {
	return !(*this == v);
}
ostream& operator<<(ostream& ostr, Vec3 const& v){
	ostr << "Vec3(" << v.x << ", " << v.y << ", " << v.z << ")";
	return ostr;
}
istream& operator>>(istream& istr, Vec3 const& v) {
	istr >> v.x >> v.y >> v.z;
	return istr;
}
float Vec3::operator[](int index) {
	return vect[index];
}
void Vec3:: test(){
}