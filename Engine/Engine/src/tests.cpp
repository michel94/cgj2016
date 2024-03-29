#include "tests.hpp"
#include "vec.hpp"
#include "mat.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>

using namespace std;

float rand01() {
	return (float)(rand() % 100000) / 100000;
}

Vec3 randVec3() {
	return Vec3(rand01(), rand01(), rand01());
}

void Tests::testVec3() {
	Vec3 v;
	float* d = (float*)v.data();
	v.x = 3;
	v.y = 4;
	v.z = 2;
	assert(v == Vec3(3, 4, 2));
	assert(d[0] == v.x && d[1] == v.y && d[2] == v.z);

	assert(Vec3(2, 1.0, 0) == Vec3(Vec2(2, 1)));
	assert(Vec2(Vec3(2, 1, 3)) == Vec2(2, 1));
	assert(Vec3(0, 1.0, 2) == Vec3(1, 1, 1) + Vec3(-1, 0, 1));
	assert(Vec3(0, 1.0, 2) == Vec3(1, 1, 1) - Vec3(1, 0, -1));
	assert(Vec3(0, 1.0, 2) == Vec3(1, 1, 1) - Vec3(1, 0, -1));
	assert(Vec3(0, 1.0, 2) == Vec3(0.0, 0.5, 1) * 2);
	assert(Vec3(0, 1.0, 2) == 2 * Vec3(0.0, 0.5, 1));
	assert(Vec3(0, 1.0, 2) == Vec3(0.0, 0.5, 1) * Vec3(2, 2, 2));
	assert(Vec3(0, 1.0, 0) == Vec3(0.0, 0.0, 1.0).cross(Vec3(1.0, 0.0, 0.0)));
	assert(0.0 == Vec3(0.0, 0.0, 1.0).dot(Vec3(1.0, 0.0, 0.0)));
	Vec3 v2(3, 2, 1);
	Vec3 v3(3, 2, 1);
	v2 = ((v2 * 1.3f) + Vec3(0.2f, 0.4f, 0.7f)).cross(Vec3(1.0f, 0.0f, 1.0f));
	v3 *= 1.3f;
	v3 += Vec3(0.2f, 0.4f, 0.7f);
	v3 = v3.cross(Vec3(1.0f, 0.0f, 1.0f));
	assert(v2 == v3);

	Vec3 v4;
	istringstream is(string("0.2 3.2 1.2"));
	is >> v4;
	assert(v4 == Vec3(0.2f, 3.2f, 1.2f));

	float n = Vec3(3.45f, 6.23f, -2.91f).normalize().norm() - 1;
	assert(n < 0.001f && n > -0.001f);

	Vec3 v6(0, 3, 5), v5;
	v5 = v6;
	v6.x += 2;
	assert(v5 != v6);


	/*Vec3::test();
	Vec2::test();

	srand((unsigned int)time(NULL));
	int t = 10;
	while (t--) {
	Vec3 i = randVec3(),
	j = randVec3(),
	k = randVec3();
	Vec3 v1 = i.cross(j.cross(k)),
	v2 = j * i.dot(k) - k * i.dot(j);
	cout << "Run " << 10 - t << ":" << endl;
	cout << i << j << k << endl;
	cout << v1 << v2 << endl;
	assert(v1 == v2);

	}*/
}

void Tests::testMat() {
	Mat4 m3(Vec4(1, 2, 3, 4), Vec4(5, 6, 7, 8), Vec4(9, 10, 11, 12), Vec4(13, 14, 15, 16));
	
	assert(m3 * 3 == 3 * m3);											// multiplication by a scalar
	assert(m3 - m3 == Mat4::zeros());
	assert(Mat4::zeros() - m3 == -m3);									// negate matrix
	assert(Mat4(m3[0].getRow(),
				m3[1].getRow(),
				m3[2].getRow(),
				m3[3].getRow()).transposed() == m3);					// operator[], getRow, transposed

	Mat3 toInv = Mat3::asRows(Vec3(1, 2, 3), Vec3(0, 1, 4), Vec3(5, 6, 0));
	Mat3 inv = toInv.inverse();
	assert(toInv == inv.inverse());										// inverse

	Mat3 mul3 = Mat3::asRows(Vec3(1, 1, 1), Vec3(2, 2, 2), Vec3(3, 3, 3));
	Mat4 mul4 = Mat4::asRows(Vec4(1, 1, 1, 1), Vec4(2, 2, 2, 2), Vec4(3, 3, 3, 3), Vec4(4, 4, 4, 4));
	
	assert(mul3 * Vec3(1, 2, 3) == Vec3(6, 12, 18));					// Vec x Mat, Mat x Vec
	assert(Vec3(1, 2, 3) * mul3 == Vec3(14, 14, 14));
	assert(mul4 * Vec4(1, 2, 3, 4) == Vec4(10, 20, 30, 40));
	assert(Vec4(1, 2, 3, 4) * mul4 == Vec4(30, 30, 30, 30));
	assert(mul4 * Vec3(4, 3, 2) == Vec4(10, 20, 30, 40));
	assert(Vec3(4, 3, 2) * mul4 == Vec4(20, 20, 20, 20));
	assert(Mat2(Vec2(1, 2), Vec2(3, 4)) * Vec2(2, 1) == Vec2(5, 8));
	assert(Vec2(2, 1) * Mat2(Vec2(1, 2), Vec2(3, 4)) == Vec2(4, 10));
	
	assert(Mat4::asRows(
			Vec4(1, 0, 0, 0),
			Vec4(0, 1, 0, 0),
			Vec4(0, 0, 1, 0),
			Vec4(3, 2, 1, 1)) == Mat4::translate(Vec3(3, 2, 1)));	// translate

	//cout << Mat4::rotateAround(Vec3(0, 1, 0), 90.0f);

	assert(Mat2(Mat3(Mat4())) == Mat2());
	assert(Mat3().determinant() == 1);
	assert(Mat4(Mat3(Mat2())) == Mat4());
	
	/*Mat3 mat1 = Mat4::rotateAround(v1, 90),
		 mat2 = Mat4::rotateAround(v2, 90),
		 mat3 = Mat4::rotateAround(v3, 90);
	cout << mat1 * v1 << endl;
	cout << mat2 * v1 << endl;
	cout << mat3 * v1 << endl;
	cout << mat1 * v2 << endl;
	cout << mat2 * v2 << endl;
	cout << mat3 * v2 << endl;
	cout << mat1 * v3 << endl;
	cout << mat2 * v3 << endl;
	cout << mat3 * v3 << endl;
	*/
	Mat3 a = Mat3::asRows(Vec3(-3, 2, -5), Vec3(-1, 0, -2), Vec3(3, -4, 1));
	assert(a.adjoint() == Mat3::asRows(Vec3(-8, 18, -4), Vec3(-5, 12, -1), Vec3(4, -6, 2)) );

	Mat3 m = Mat3::asRows(Vec3(0, 0, 1), Vec3(0, 1, 0), Vec3(1, 0, 0));
	assert(m.inverse() == m);
}

void Tests::runTests() {
	//cout << setprecision(10);

	testVec2();
	testVec3();
	testVec4();
	testMat();
}
