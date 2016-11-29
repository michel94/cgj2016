#pragma once

#include "model.hpp"
#include "singleton.hpp"

class ModelManager : public Singleton<ModelManager > {
public:
	Model* getTriangle(Vec4 color);
	Model* getSquare();
	Model* getParallelogram(Vec4 color);

	Model* loadObj(string path);
	Model* getObj(string path);

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
};

typedef struct {
	GLint v_index[3];
	GLint t_index[3];
	GLint n_index[3];
} Face;


const string MODEL_DIR = "res/meshes/";

//static Vec4 lb, lt, rb, rt;
