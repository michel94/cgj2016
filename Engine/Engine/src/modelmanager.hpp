#pragma once

#include "model.hpp"
#include "singleton.hpp"

class ModelManager : public Manager<ModelManager, Model*> {
public:
	Model* getTriangle(Vec4 color);
	Model* getSquare();
	Model* getParallelogram(Vec4 color);

	Model* loadObj(string path);
	Model* getObj(string name);
	virtual Model* loadItem(string path) {
		return loadObj(path);
	}
	
	void generateTangents();
	void parseVertex(std::stringstream& sin);
	void parseTexcoord(std::stringstream& sin);
	void parseNormal(std::stringstream& sin);
	void parseFace(std::stringstream& sin);
	void parseLine(std::stringstream& sin);
	void processMeshData();
	void freeMeshData();

	Model* m;
private:
	map<string, Model*> models;
	void getPointData(int i, unsigned int& vi, Vec3& p, Vec2& t);
};

typedef struct {
	GLint v_index[3];
	GLint t_index[3];
	GLint n_index[3];
} Face;

