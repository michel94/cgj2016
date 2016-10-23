#include "mat.hpp"
#include <string.h>
#include <math.h>
#include <iostream>

using namespace std;

const float PI = 3.14159265358979f;

template<class Sub>
Mat<Sub>::~Mat() {
}
template<class Sub>
Mat<Sub>::Mat(const Sub& other) {
	data = new float[size*size];
	size = other.size;
	memcpy(data, other.data, size*size*sizeof(float));
}
template<class Sub>
Mat<Sub>::Mat(int size) {
	data = new float[size*size];
	memset(data, 0, size*size * sizeof(float));
	for (int i = 0; i < size*size; i++)
		if (i / size == i%size)
			data[i] = 1;
	this->size = size;
}
template<class Sub>
Mat<Sub>::Mat(vector<float*>& cols){
	size = (int)cols.size();
    data = new float[size*size];
	for (int i = 0; i < size; i++)
		memcpy(&data[size*i], &cols[i][0], size * sizeof(float));
}

template<class Sub>
void Mat<Sub>::operator=(const Sub& other) {
	memcpy(data, other.data, size*size*sizeof(float));
}
template<class Sub>
Sub Mat<Sub>::operator+(const Sub& other){
	Sub m = Sub();
    for(int i=0; i<size*size; i++)
        m.data[i] = data[i] + other.data[i];
    return m;
}
template<class Sub>
void Mat<Sub>::operator+=(const Sub& other){
    for(int i=0; i<size*size; i++)
        data[i] += other.data[i];
}
template<class Sub>
Sub Mat<Sub>::operator-(const Sub& other) {
	Sub m = Sub();
	for (int i = 0; i<size*size; i++)
		m.data[i] = data[i] - other.data[i];
	return m;
}
template<class Sub>
Sub Mat<Sub>::operator-(){
	Sub m;
	for (int i = 0; i < size*size; i++)
		m.data[i] = -data[i];
	return m;
}
template<class Sub>
void Mat<Sub>::operator-=(const Sub& other) {
	for (int i = 0; i<size*size; i++)
		data[i] -= other.data[i];
}
template<class Sub>
bool Mat<Sub>::operator==(const Sub& other) {
	for (int i = 0; i < size*size; i++) {
		if (abs(data[i] - other.data[i]) > 0.000001)
			return false;
	}
	return true;
}
template<class Sub>
Sub Mat<Sub>::operator*(float s) {
	Sub m;
	for (int i = 0; i < size*size; i++)
		m.data[i] = data[i] * s;
	return m;
}

template<class Sub>
Sub Mat<Sub>::operator*(const Sub& other){
	Sub m;
	memset(m.data, 0, m.size*m.size*sizeof(float));
    for(int y=0; y<size; y++){
        for(int x=0; x<size; x++){
            for(int i=0; i<size; i++){
				m.data[x*size+y] += data[x*size+i] * other.data[i*size + y];
            }
        }
    }
    return m;
}
template<class Sub>
float* Mat<Sub>::matXVec(float* v){
	float *res = new float[size];
	memset(res, 0, size*sizeof(float));
	for (int row = 0; row < size; row++)
		for (int col = 0; col < size; col++)
			res[row] += data[row + col*size] * v[col];
	return res;
}

template<class Sub>
float* Mat<Sub>::vecXMat(float* v){
	float* res = new float[size];
	memset(res, 0, size*sizeof(float));
	for (int col = 0; col < size; col++)
		for (int row = 0; row < size; row++)
			res[col] += data[row + col*size] * v[row];
	return res;
}

template<class Sub>
void Mat<Sub>::operator*=(float s){
	for (int i = 0; i < size*size; i++)
		data[i] *= s;
}
template<class Sub>
Sub operator*=(float s, const Sub & other){
	other *= s;
}

template<class Sub>
void Mat<Sub>::transpose() {
	for (int i = 0; i < size; i++)
		for (int j = 0; j < i; j++)
			swap(data[i*size + j], data[j*size + i]);
}
template<class Sub>
Sub Mat<Sub>::transposed() {
	Sub m;
	for (int i = 0; i < size; i++)
		for (int j = 0; j <= i; j++) {
			m.data[j*size + i] = data[i*size + j];
			m.data[i*size + j] = data[j*size + i];
		}
	return m;
}
template<class Sub>
void Mat<Sub>::invert(){
	Sub m = inverse();
	memcpy(data, m.data, size*size*sizeof(float));
}

template<class Sub>
void Mat<Sub>::copySquare(Mat& dest, Vec2 d, Mat& src, Vec2 s, Vec2 size) {
	Vec2 offset = d-s;
	for (int i = (int)s.x; i < (int)(s.x + size.x); i++) {
		for (int j = (int)s.y; j < (int)(s.y + size.y); j++) {
			Vec2 t = Vec2(i, j) + offset;
			dest[(int)t.x][(int)t.y] = src[i][j];
		}
	}
}

template<class Sub>
Sub Mat<Sub>::adjoint() {
	Sub adj;
	if (adj.size > 2) {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				Mat m(size - 1);
				copySquare(m, Vec2(0, 0), *this, Vec2(0, 0), Vec2((float)i, (float)j));
				copySquare(m, Vec2(i, 0), *this, Vec2(i + 1, 0), Vec2(size - i - 1, j));
				copySquare(m, Vec2(0, j), *this, Vec2(0, j + 1), Vec2(i, size - j - 1));
				copySquare(m, Vec2(i, j), *this, Vec2(i + 1, j + 1), Vec2(size - i - 1, size - j - 1));
				int signal = (i + j) % 2 ? -1 : 1;
				adj[i][j] = signal * m.determinant();
			}
		}
	}
	else if (adj.size == 2) {
		float temp[] = { data[3], -data[2], -data[1], data[0] };
		memcpy(adj.data, temp, sizeof(float) * 4);
	}
	adj.transpose();
	return adj;
}

template<class Sub>
Sub Mat<Sub>::inverse() {
	Sub adj = adjoint();
	adj *= 1.0f/determinant();
	return adj;
}

template<class Sub>
void Mat<Sub>::matrixPower(map<int, Sub >& dict, int n) {
	if (n == 1)
		return;
		
	if (dict.find(n) != dict.end())
		return;
	
	matrixPower(dict, n / 2);
	if (n % 2 == 1) {
		matrixPower(dict, (n + 1) / 2);
	}
	
	dict[n] = dict[n / 2] * dict[(n + 1) / 2];
}

template<class Sub>
Sub Mat<Sub>::operator^(int n) {
	Sub m;
	memcpy(m.data, this->data, size * size * sizeof(float));
	map<int, Sub> dict;
	dict[1] = m;
	matrixPower(dict, n);
	return dict[n];
}

template<class Sub>
void Mat<Sub>::operator*=(const Sub& m) {
	float* temp = new float[size*size];
	memset(temp, 0, size*size*sizeof(float));
	swap(temp, data);
	for (int y = 0; y<size; y++) {
		for (int x = 0; x<size; x++) {
			for (int i = 0; i<size; i++) {
				data[x*size + y] += temp[x*size + i] * m.data[i*size + y];
			}
		}
	}
	delete[] temp;
}
template<class Sub>
float Mat<Sub>::determinant(){
	if (size == 1)
		return data[0];
	else if(size == 2)
        return data[0] * data[3] - data[1] * data[2];
	else if (size == 3) {
		return data[0] * (data[4] * data[8] - data[5] * data[7])
			- data[3] * (data[1] * data[8] - data[2] * data[7])
			+ data[6] * (data[1] * data[5] - data[2] * data[4]);
	}else{
        float s=0;
        vector<float*> cols;
		cols.resize(size - 1);
        for(int i = 1; i<size; i++){
            cols[i-1] = &data[i*size + 1];
        }
        for(int i = 0; i<size; i++){
			int signal = (i % 2) ? -1 : 1;
			Mat t = Mat(cols);
			float c = signal * t.determinant();
			s += c;

			if(i < size-1)
				cols[i] = data + i*size + 1;
			else
				cols[i-1] = data + i*size + 1;
        }
        return s;
    }
}
template<class Sub>
Sub Mat<Sub>::rows(vector<Vec*> rows) {
	Sub m;
	for (int col = 0; col < m.size; col++) {
		for (int i = 0; i < rows.size(); i++) {
			m.data[col*m.size + i] = (*rows[i])[col];
		}
	}
	return m;
}


Mat2::Mat2() : Mat<Mat2>(2) {
}

Mat2::Mat2(const Mat3& m) : Mat<Mat2>(2) {
	for (int i = 0; i < size; i++)
		memcpy(&data[size * i], &m.data[m.size*i], size * sizeof(float));
}

Mat2::Mat2(float col1[], float col2[], float col3[]) : Mat<Mat2>(2) {
	memcpy(&data[size * 0], col1, size * sizeof(float));
	memcpy(&data[size * 1], col2, size * sizeof(float));
}
Mat2::Mat2(Vec2 v1, Vec2 v2) : Mat<Mat2>(2) {
	memcpy(&data[size * 0], v1.data(), v1.size());
	memcpy(&data[size * 1], v2.data(), v2.size());
}
void Mat2::operator=(const Mat2& other) {
	Mat<Mat2>::operator=(other);
}
Mat2 Mat2::asRows(Vec2 v1, Vec2 v2) {
	vector<Vec*> rows = { (Vec*)&v1, (Vec*)&v2};
	return Mat2::rows(rows);
}


Mat3::Mat3() : Mat<Mat3>(3) {
}

Mat3::Mat3(const Mat4& m) : Mat<Mat3>(3) {
	for (int i = 0; i < size; i++)
		memcpy(&data[size * i], &m.data[m.size*i], size * sizeof(float));
}

Mat3::Mat3(const Mat2 & m) : Mat<Mat3>(3) {
	for (int i = 0; i < m.size; i++)
		memcpy(&(data[size * i]), &(m.data[m.size * i]), sizeof(float)*m.size);
}

Mat3::Mat3(float col1[], float col2[], float col3[]) : Mat<Mat3>(3) {
	memcpy(&data[size * 0], col1, size * sizeof(float));
	memcpy(&data[size * 1], col2, size * sizeof(float));
	memcpy(&data[size * 2], col3, size * sizeof(float));
}
Mat3::Mat3(Vec3 v1, Vec3 v2, Vec3 v3) : Mat<Mat3>(3) {
	memcpy(&data[size * 0], v1.data(), v1.size());
	memcpy(&data[size * 1], v2.data(), v2.size());
	memcpy(&data[size * 2], v3.data(), v3.size());
}
void Mat3::operator=(const Mat3& other) {
	Mat<Mat3>::operator=(other);
}
Mat3 Mat3::asRows(Vec3 v1, Vec3 v2, Vec3 v3) {
	vector<Vec*> rows = { (Vec*)&v1, (Vec*)&v2, (Vec*)&v3};
	return Mat3::rows(rows);
}
Mat3 Mat3::dual(Vec3 a){
	return Mat3::asRows(
		Vec3(0.0f, -a.z, a.y),
		Vec3(a.z, 0.0f, -a.x),
		Vec3(-a.y, a.x, 0.0f)
	);
}


Mat4::Mat4() : Mat<Mat4>(4) {
}

Mat4::Mat4(const Mat3& m) : Mat<Mat4>(4) {
	for (int i = 0; i < m.size; i++)
		memcpy(&(data[size * i]), &(m.data[m.size * i]), sizeof(float)*m.size);
}

Mat4::Mat4(float col1[], float col2[], float col3[], float col4[]) : Mat<Mat4>(4) {
	memcpy(&data[size * 0], col1, size * sizeof(float));
	memcpy(&data[size * 1], col2, size * sizeof(float));
	memcpy(&data[size * 2], col3, size * sizeof(float));
	memcpy(&data[size * 3], col4, size * sizeof(float));
}
Mat4::Mat4(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4) : Mat<Mat4>(4) {
	memcpy(&data[size * 0], v1.data(), v1.size());
	memcpy(&data[size * 1], v2.data(), v2.size());
	memcpy(&data[size * 2], v3.data(), v3.size());
	memcpy(&data[size * 3], v4.data(), v4.size());
}

void Mat4::operator=(const Mat4& other) {
	Mat<Mat4>::operator=(other);
}

Mat4 Mat4::translate(Vec3 v) {
	Mat4 m;
	m.data[3] = v.x;
	m.data[7] = v.y;
	m.data[11] = v.z;
	return m;
}
Mat4 Mat4::scale(Vec3 v) {
	Mat4 m;
	m.data[0] = v.x;
	m.data[5] = v.y;
	m.data[10] = v.z;
	return m;
}

Mat4 Mat4::rotateAround(Vec3 v, float angle) {
	Mat3 m, A = Mat3::dual(v.normalize());
	angle = (angle / 180) * PI;
	m += (A*sin(angle)) + (1 - cos(angle)) * (A*A);
	return Mat4(m);
}

Mat4 Mat4::asRows(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4) {
	vector<Vec*> rows = { (Vec*)&v1, (Vec*)&v2, (Vec*)&v3, (Vec*)&v4 };
	return Mat4::rows(rows);
}

// Hacked operators

Vec2 dataToVec2(float* r) {
	Vec2 v;
	v.setData(r);
	delete[] r;
	return v;
}
Vec3 dataToVec3(float* r) {
	Vec3 v;
	v.setData(r);
	delete[] r;
	return v;
}
Vec4 dataToVec4(float* r) {
	Vec4 v;
	v.setData(r);
	delete[] r;
	return v;
}

Vec2 operator*(Mat2 m, Vec2 v){
	float* c = m.matXVec((float*)v.data());
	return dataToVec2(c);
}

Vec2 operator*(Vec2 v, Mat2 m){
	float* c = m.vecXMat((float*)v.data());
	return dataToVec2(c);
}

// Mat3 x Vec3
Vec3 operator*(Mat3 m, Vec3 v) {
	float* c = m.matXVec((float*)v.data());
	return dataToVec3(c);
}
// Mat4 x Vec3
Vec4 operator*(Mat4 m, Vec3 v){
	float* c = m.matXVec((float*)Vec4(v).data());
	return dataToVec4(c);
}
// Mat4 x Vec4
Vec4 operator*(Mat4 m, Vec4 v){
	float* c = m.matXVec((float*)Vec4(v).data());
	return dataToVec4(c);
}

// Vec3 x Mat3
Vec3 operator*(Vec3 v, Mat3 m) {
	float* c = m.vecXMat((float*)v.data());
	return dataToVec3(c);
}
// Vec3 x Mat4
Vec4 operator*(Vec3 v, Mat4 m) {
	float* c = m.vecXMat((float*)Vec4(v).data());
	return dataToVec4(c);
}
// Vec4 x Mat4
Vec4 operator*(Vec4 v, Mat4 m) {
	float* c = m.vecXMat((float*)Vec4(v).data());
	return dataToVec4(c);
}
