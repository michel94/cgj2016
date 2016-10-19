#pragma once

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

class Vec {
public:
	virtual void setData(float*) = 0;
	virtual void* data() const = 0;
	virtual int size() const = 0;
	virtual float operator[](int index) = 0;
};

class Vec3;

class Vec2 : public Vec{
public:
	Vec2();
	Vec2(int x, int y);
	Vec2(float x, float y);
	Vec2(double x, double y);
	Vec2(const Vec2& v);
	Vec2(const Vec3& v);
	virtual void* data() const;
	virtual void setData(float* data);
	virtual int size() const;

	void normalized();
	Vec2 normalize() const ;
	float norm() const;
	float length() const ;
	float quadrance() const;
	float dot(const Vec2& v) const;
	
	Vec2 operator+(const Vec2& v) const ;
	Vec2 operator-(const Vec2& v) const ;
	Vec2 operator-() const;
	Vec2 operator*(float s) const ;
	friend Vec2 operator*(float s, Vec2& v) {
		return Vec2(s * v.x, s * v.y);
	}
	Vec2 operator*(const Vec2& v) const;
	void operator=(const Vec2& v);
	bool operator==(const Vec2& v);
	bool operator!=(const Vec2& v);
	void operator+=(const Vec2& v);
	void operator-=(const Vec2& v);
	void operator*=(float s);
	void operator*=(const Vec2& v);
	virtual float operator[](int index);

	friend ostream& operator<<(ostream& ostr, Vec2 const& v);
	friend istream& operator>>(istream& istr, Vec2 const& v);
	static void test();

	float &x=vect[0], &y=vect[1];

private:
	float vect[2];
};


class Vec3 : public Vec {
public:
	Vec3();
	Vec3(float x, float y, float z);
	Vec3(const Vec2& v);
	Vec3(const Vec3& v);
	void* data() const;
	virtual void setData(float* data);
	int size() const;
	void normalized();
	Vec3 normalize() const ;
	float norm() const;
	float quadrance() const;
	float length() const ;
	bool operator<(const Vec3& v) const ;
	bool operator<=(const Vec3& v) const ;
	bool operator>(const Vec3& v) const ;
	bool operator>=(const Vec3& v) const ;
	Vec3 operator+(const Vec3& v) const ;
	Vec3 operator-(const Vec3& v) const ;
	Vec3 operator-() const;
	Vec3 operator*(float s) const;
	friend Vec3 operator*(float s, Vec3& v);
	Vec3 operator*(const Vec3& v) const;

	void operator+=(const Vec3& v);
	void operator-=(const Vec3& v);
	void operator*=(float s);
	void operator*=(const Vec3& v);

	Vec3 cross(const Vec3& v) const;
	float dot(const Vec3& v) const;
	void operator=(const Vec3& v);
	bool operator==(const Vec3& v);
	bool operator!=(const Vec3& v);
	friend ostream& operator<<(ostream& ostr, Vec3 const& v);
	friend istream& operator>>(istream& istr, Vec3 const& v);
	virtual float operator[](int index);
	static void test();

	float &x=vect[0], &y=vect[1], &z=vect[2];

private:
	float vect[3];
};

class Vec4 : public Vec {
public:
	Vec4();
	Vec4(float x, float y, float z, float w);
	Vec4(const Vec3& v);
	Vec4(const Vec4& v);
	virtual void setData(float* data);
	void* data() const;
	int size() const;
	void normalized();
	Vec4 normalize() const ;
	float norm() const;
	float quadrance() const;
	float length() const ;
	bool operator<(const Vec4& v) const ;
	bool operator<=(const Vec4& v) const ;
	bool operator>(const Vec4& v) const ;
	bool operator>=(const Vec4& v) const ;
	Vec4 operator+(const Vec4& v) const ;
	Vec4 operator-(const Vec4& v) const ;
	Vec4 operator-() const;
	Vec4 operator*(float s) const ;
	friend Vec4 operator*(float s, Vec4& v);
	Vec4 operator*(const Vec4& v) const;

	void operator+=(const Vec4& v);
	void operator-=(const Vec4& v);
	void operator*=(float s);
	void operator*=(const Vec4& v);
	float dot(const Vec4& v) const;
	void operator=(const Vec4& v);
	bool operator==(const Vec4& v);
	bool operator!=(const Vec4& v);
	friend ostream& operator<<(ostream& ostr, Vec4 const& v);
	friend istream& operator>>(istream& istr, Vec4 const& v);
	virtual float operator[](int index);

	float &x=vect[0], &y=vect[1], &z=vect[2], &w=vect[3];

private:
	float vect[4];
};
