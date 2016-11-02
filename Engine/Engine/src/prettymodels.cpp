
#include "prettymodels.hpp"

map<Vec4, vector<Vec3> > colors =
{	{Vec4(1, 0, 0, 1), {Vec3(255, 0, 0), Vec3(193, 138, 138), Vec3(162, 42, 42), Vec3(215, 86, 86), Vec3(94, 44, 44), Vec3(247, 141, 141)}},
	{Vec4(1, 0, 1, 1),{ Vec3(255, 0, 255), Vec3(255, 112, 255), Vec3(154, 91, 159), Vec3(118, 28, 125), Vec3(241, 178, 245), Vec3(80, 24, 84)} },
	{Vec4(1, 1, 0, 1),{ Vec3(255, 255, 0), Vec3(179, 179, 0), Vec3(255, 255, 133), Vec3(218, 218, 169), Vec3(88, 88, 35), Vec3(139, 139, 80) } },
	{Vec4(0, 1, 0, 1),{ Vec3(0, 255, 0), Vec3(0, 128, 0), Vec3(15, 87, 15), Vec3(33, 69, 33), Vec3(35, 197, 135), Vec3(105, 226, 105) } },
	{Vec4(1,0.5f,0, 1),{ Vec3(255, 122, 0), Vec3(255, 171, 92), Vec3(204, 99, 0), Vec3(92, 44, 0), Vec3(181, 118, 59), Vec3(142, 106, 72) } },
	{Vec4(0, 0, 1, 1),{ Vec3(0, 0, 255), Vec3(93, 93, 162), Vec3(66, 66, 189), Vec3(110, 110, 242), Vec3(10, 10, 112), Vec3(42, 42, 91) } },
	{Vec4(0, 1, 1, 1),{ Vec3(0, 255, 255), Vec3(122, 255, 253), Vec3(0, 168, 165), Vec3(7, 95, 94), Vec3(137, 190, 189), Vec3(151, 231, 230) } }
};

PrettyModel::PrettyModel(vector<Vec4> vertices, vector<GLubyte> indices, Vec4 color) :  Model()/*: Model(vertices, indices, color)*/{
	createData(vertices, color);
	createBuffers(data, (int)vertices.size(), indices);
}

void PrettyModel::createData(vector<Vec4>& vertices, Vec4 color) {
	data = new Vertex[vertices.size()];
	int nTopTriangles = ((int)vertices.size() + 12) / 12 - 2;
	
	Vec4 c;
	for (int i = 0; i < nTopTriangles*6; i++) {
		if (colors.find(color) != colors.end()) {
			int isFront = i < nTopTriangles * 3 ? 0 : 1;
			c = colors[color][isFront] * (1.0f / 255);
		}else
			c = Vec4(randF(), randF(), randF(), randF());
		memcpy(data[i].XYZW, vertices[i].data(), vertices[i].size());
		memcpy(data[i].RGBA, c.data(), c.size());
	}
	
	for (int i = nTopTriangles * 6; i < vertices.size(); i++) {
		int j = i - nTopTriangles * 6;
		
		if (colors.find(color) != colors.end()) {
			c = colors[color][j / 6 + 2] * (1.0f / 255);
		}else
			c = Vec4(randF(), randF(), randF(), randF());
		memcpy(data[i].XYZW, vertices[i].data(), vertices[i].size());
		memcpy(data[i].RGBA, c.data(), c.size());
	}
}