#include "modelmanager.hpp"

const string MODEL_DIR = "res/meshes/";

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
	return getItem(MODEL_DIR + name + ".obj");
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
	if (m->NormalsLoaded && m->TexcoordsLoaded) {
		m->TangentsLoaded = true;
		generateTangents();
	}
	processMeshData();
	
	m->createBuffers();
	freeMeshData();

	return m;
}

Vec3 toVec3(Normal& v) {
	return Vec3(v.nx, v.ny, v.nz);
}

Vec3 toVec3(Vertex& v) {
	return Vec3(v.x, v.y, v.z);
}
Vec2 toVec2(Texcoord& v) {
	return Vec2(v.u, v.v);
}

void ModelManager::getPointData(int i, unsigned int& vi, Vec3& p, Vec2& t){
	vi = m->vertexIdx[i] - 1;
	p = toVec3(m->vertexData[vi]);
	unsigned int ti = m->texcoordIdx[i]-1;
	t = toVec2(m->texcoordData[ti]);
	//unsigned int ni = m->normalIdx[i]-1;
	//n = toVec3(m->normalData[ni]);
}

void ModelManager::generateTangents(){
	vector<Vec3> tangents;
	tangents.resize(m->vertexData.size());
	vector<Vec3> bitangents;
	bitangents.resize(m->vertexData.size());

	for (unsigned int i = 0; i < m->vertexIdx.size(); i+=3) {
		unsigned int i0, i1, i2;
		Vec3 p0, p1, p2;
		Vec2 t0, t1, t2;
		getPointData(i, i0, p0, t0);
		getPointData(i+1, i1, p1, t1);
		getPointData(i+2, i2, p2, t2);
		
		Vec3 Edge1 = p1 - p0;
		Vec3 Edge2 = p2 - p0;

		Vec2 Delta1 = t1 - t0;
		Vec2 Delta2 = t2 - t0;

		float f = 1.0f / (Delta1.x * Delta2.y - Delta2.x * Delta1.y);

		Vec3 Tangent, Bitangent;


		Tangent.x = f * (Delta2.y * Edge1.x - Delta1.y * Edge2.x);
		Tangent.y = f * (Delta2.y * Edge1.y - Delta1.y * Edge2.y);
		Tangent.z = f * (Delta2.y * Edge1.z - Delta1.y * Edge2.z);

		Bitangent.x = f * (-Delta2.x * Edge1.x - Delta1.x * Edge2.x);
		Bitangent.y = f * (-Delta2.x * Edge1.y - Delta1.x * Edge2.y);
		Bitangent.z = f * (-Delta2.x * Edge1.z - Delta1.x * Edge2.z);

		tangents[i0] += Tangent;
		tangents[i1] += Tangent;
		tangents[i2] += Tangent;
		bitangents[i0] += Bitangent;
		bitangents[i1] += Bitangent;
		bitangents[i2] += Bitangent;
	}

	m->tangentsData.resize(tangents.size());
	m->bitangentsData.resize(bitangents.size());
	for (unsigned int i = 0 ; i < tangents.size(); i++) {
		tangents[i].normalized();
		bitangents[i].normalized();
		m->tangentsData[i] = {tangents[i].x, tangents[i].y, tangents[i].z};
		m->bitangentsData[i] = { bitangents[i].x, bitangents[i].y, bitangents[i].z };
		cout << "Vertex " << i << ": " << toVec3(m->vertexData[i]) << endl;
		cout << "Tangent " << tangents[i] << endl;
		cout << "Bitangent " << bitangents[i] << endl;
		cout << tangents[i].dot(bitangents[i]) << endl;

	}
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
		if (m->TangentsLoaded) {
			Vertex v = m->tangentsData[vi - 1];
			m->Tangents.push_back(v);
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

