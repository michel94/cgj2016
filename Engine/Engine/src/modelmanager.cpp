#include "modelmanager.hpp"
Vertex lb = { 0.0f, 0.0f, 0.0f},
	lt = {0.0f, 1.0f, 0.0f},
	rb = {1.0f, 0.0f, 0.0f},
	rt = {1.0f, 1.0f, 0.0f};

Model* ModelManager::getTriangle(Vec4 color) {
	if (models.find("triangle") == ModelManager::models.end()) {
		Model* m = new Model();
		m->Vertices = { lb,rb,lt };
		m->createBuffers();
		models["triangle"] = m;
	}
	return models["triangle"];
	

}
Model* ModelManager::getSquare() {
	if (models.find("square") == models.end()) {
		Model* m = new Model();
		m->Vertices = { lb, rb, lt, lt, rb, rt };
		m->createBuffers();
		models["square"] = m;
	}
	return models["square"];
}

Model* ModelManager::getParallelogram(Vec4 color) {
	if (models.find("parallelogram") == models.end()) {
		Model* m = new Model();
		m->Vertices = { rb, {1.5f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, lb, rb, {0.5f, 0.5f, 0.0f} };
		m->createBuffers();
		models["parallelogram"] = m;
	}
	return models["parallelogram"];
}

Model* ModelManager::getObj(string name) {
	if (models.find(name) == models.end()) {
		Model* model = loadObj(MODEL_DIR + name + ".obj");
		models[name] = model;
	}
	return models[name];
}


Model* ModelManager::loadObj(std::string filename){
	m = new Model();

	std::ifstream ifile(filename);
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		parseLine(std::stringstream(line));
	}
	m->TexcoordsLoaded = (m->texcoordIdx.size() > 0);
	m->NormalsLoaded = (m->normalIdx.size() > 0);
	processMeshData();
	
	m->createBuffers();
	freeMeshData();

	return m;
}


void ModelManager::parseVertex(std::stringstream& sin)
{
	Vertex v;
	sin >> v.x >> v.y >> v.z;
	m->vertexData.push_back(v);
}

void ModelManager::parseTexcoord(std::stringstream& sin)
{
	Texcoord t;
	sin >> t.u >> t.v;
	m->texcoordData.push_back(t);
}

void ModelManager::parseNormal(std::stringstream& sin)
{
	Normal n;
	sin >> n.nx >> n.ny >> n.nz;
	m->normalData.push_back(n);
}

void ModelManager::parseFace(std::stringstream& sin)
{
	std::string token;
	for (int i = 0; i < 3; i++) {
		std::getline(sin, token, '/');
		if (token.size() > 0) m->vertexIdx.push_back(std::stoi(token));
		std::getline(sin, token, '/');
		if (token.size() > 0) m->texcoordIdx.push_back(std::stoi(token));
		std::getline(sin, token, ' ');
		if (token.size() > 0) m->normalIdx.push_back(std::stoi(token));
	}
}

void ModelManager::parseLine(std::stringstream& sin)
{
	std::string s;
	sin >> s;
	if (s.compare("v") == 0) parseVertex(sin);
	else if (s.compare("vt") == 0) parseTexcoord(sin);
	else if (s.compare("vn") == 0) parseNormal(sin);
	else if (s.compare("f") == 0) parseFace(sin);
}



void ModelManager::processMeshData()
{
	for (unsigned int i = 0; i < m->vertexIdx.size(); i++) {
		unsigned int vi = m->vertexIdx[i];
		Vertex v = m->vertexData[vi - 1];
		m->Vertices.push_back(v);
		if (m->TexcoordsLoaded) {
			unsigned int ti = m->texcoordIdx[i];
			Texcoord t = m->texcoordData[ti - 1];
			m->Texcoords.push_back(t);
		}
		if (m->NormalsLoaded) {
			unsigned int ni = m->normalIdx[i];
			Normal n = m->normalData[ni - 1];
			m->Normals.push_back(n);
		}
	}
}

void ModelManager::freeMeshData()
{
	m->vertexData.clear();
	m->texcoordData.clear();
	m->normalData.clear();
	m->vertexIdx.clear();
	m->texcoordIdx.clear();
	m->normalIdx.clear();
}

