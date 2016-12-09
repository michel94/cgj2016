#pragma once

#include <sstream>
#include <vector>
#include <map>
#include "vec.hpp"

using namespace std;

class Mat3;
class Mat4;

template<class Sub>
class Mat {
public:
	class RowPointer {
	private:
		Mat<Sub>* m;
		int row;
	public:
		RowPointer(Mat<Sub>* m, int row) {
			this->m = m;
			this->row = row;
		}
		float& operator[](int col) {
			return (*m).data[col*m->size + row];
		}
		float* getRow() {
			int size = m->size;
			float *rowData = new float[size];
			for (int i = 0; i < size; i++)
				rowData[i] = (*m).data[row + size*i];
			return rowData;
		}
	};

	Mat(const Sub& m);
	//Mat(const Mat<Sub>& other);
	Mat(vector<float*>& cols);
	int size;
	void operator=(const Sub& other);
	virtual Sub operator+(const Sub& other);
	virtual void operator+=(const Sub& other);
	virtual Sub operator-(const Sub& other);
	virtual Sub operator-();
	virtual void operator-=(const Sub& other);
	virtual Sub operator^(int n);
	virtual Sub operator*(float s);
	virtual Sub operator*(const Sub& other);
	float* vecXMat(float *v);
	float* matXVec(float *v);
	friend void operator*=(float s, const Sub& other);
	friend Sub operator*(float s, const Sub& other) {
		Sub m;
		for (int i = 0; i < m.size*m.size; i++)
			m.data[i] = s * other.data[i];
		return m;
	}
	virtual void operator*=(float s);
	
	//virtual float* operator*(const Vec& v);
	//friend float* operator*(const Vec& v, const Sub& other);
	virtual int byteSize() {
		return size * size * sizeof(float);
	}
	virtual void transpose();
	virtual Sub transposed();
	virtual Sub adjoint();
	virtual void invert();
	virtual Sub inverse();
	virtual float determinant();
	
	virtual void operator*=(const Sub& other);
	virtual bool operator==(const Sub& other);
	friend inline ostream& operator<<(ostream& ostr, Mat<Sub> const& m) {
		ostr << "Mat" << m.size << "(";
		for (int i = 0; i < m.size; i++) {
			if (i > 0)
				ostr << "     ";
			for (int j = 0; j < m.size; j++) {
				ostr << m.data[j * m.size + i] << ", ";
			}
			if (i < m.size - 1)
				ostr << endl;
		}
		ostr << ")" << endl;
		return ostr;
	}
	friend inline istream& operator>>(istream & istr, Sub const& m){
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				istr >> m.data[j * 4 + i];
			}
		}
		return istr;
	}
	RowPointer operator[](int row) {
		return RowPointer(this, row);
	}

	static Sub rows(vector<Vec*> rows);
	static Sub zeros() {
		Sub m;
		memset(m.data, 0, m.size*m.size*sizeof(float));
		return m;
	}
	static Sub identity() {
		Sub m;
		return m;
	}
	virtual ~Mat();
	
	float* data;
protected:
	Mat(int size);
private:
	void matrixPower(map<int, Sub >& dict, int n);
	void copySquare(Mat& dest, Vec2 d, Mat& src, Vec2 s, Vec2 size);
};

class Mat2 : public Mat<Mat2> {
public:
	Mat2();
	Mat2(const Mat3 & m);
	Mat2(float col1[], float col2[], float col3[]);
	Mat2(Vec2 v1, Vec2 v2);
	void operator=(const Mat2 & other);
	Mat2(const Mat2& m) : Mat<Mat2>(2) {
		memcpy(data, m.data, size*size*sizeof(float));
	}
	~Mat2() {
		delete[] data;
	}

	// static functions
	static Mat2 asRows(Vec2 v1, Vec2 v2);
};

class Mat3 : public Mat<Mat3> {
public:
	Mat3();
	Mat3(const Mat4& m);
	Mat3(const Mat2& m);
	Mat3(float col1[], float col2[], float col3[]);
	Mat3(Vec3 v1, Vec3 v2, Vec3 v3);
	void operator=(const Mat3 & other);
	Mat3(const Mat3& m) : Mat<Mat3>(3){
		memcpy(data, m.data, size*size*sizeof(float));
	}
	~Mat3() {
		delete[] data;
	}
	
	// static functions
	static Mat3 asRows(Vec3 v1, Vec3 v2, Vec3 v3);
	static Mat3 dual(Vec3 v);
};

class Mat4 : public Mat<Mat4> {
public:
    Mat4();
	Mat4(const Mat4& m) : Mat<Mat4>(4) {
		memcpy(data, m.data, size*size*sizeof(float));
	}
	Mat4(const Mat3& m);
    Mat4(float col1[], float col2[], float col3[], float col4[]);
	Mat4(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4);
	void operator=(const Mat4& other);
	~Mat4() {
		delete[] data;
	}
	
	// static functions
	static Mat4 translate(Vec3 v);
	static Mat4 scale(Vec3 v);
	static Mat4 rotateAround(Vec3 v, float angle);
	static Mat4 lookAt(Vec3 eye, Vec3 lookTo, Vec3 up);
	static Mat4 ortho(float left, float right, float bottom, float top, float near, float far);
	static Mat4 perspective(float fovy, float aspect, float zNear, float zFar);

	static Mat4 asRows(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4);

};

Vec2 operator*(Mat2 m, Vec2 v);
Vec2 operator*(Vec2 v, Mat2 m);
Vec3 operator*(Mat3 m, Vec3 v);
Vec4 operator*(Mat4 m, Vec3 v);
Vec4 operator*(Mat4 m, Vec4 v);
Vec3 operator*(Vec3 v, Mat3 m);
Vec4 operator*(Vec3 v, Mat4 m);
Vec4 operator*(Vec4 v, Mat4 m);
