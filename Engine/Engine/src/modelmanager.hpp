#pragma once

#include "model.hpp"

template<typename M = Model>
class ModelManager {
public:
	static Model* getTriangle(Vec4 color);
	static Model* getSquare(Vec4 color);
	static Model* getParallelogram(Vec4 color);

	static Model* getPrism(vector<Vec4>& topFace, Vec4 color);
	static Model* getTriangularPrism(Vec4 color);
	static Model* getSquarePrism(Vec4 color);
	static Model* getParallelogramPrism(Vec4 color);

	static Model* loadObj(string path);
	static Model* getObj(string path);
private:
	static map<string, Model*> models;
};

template<typename M>
Model* ModelManager<M>::getPrism(vector<Vec4>& topFace, Vec4 color) {
	vector<Vec4> vertices;
	vector<GLubyte> indices;
	float h = 1;

	int size = (int)topFace.size();
	for (int i = 1; i < size - 1; i++) {
		vertices.push_back(topFace[0]);
		vertices.push_back(topFace[i + 1]);
		vertices.push_back(topFace[i]);

	}
	for (int i = 1; i < size - 1; i++) {
		vertices.push_back(topFace[0] + Vec4(0, 0, h, 0));
		vertices.push_back(topFace[i] + Vec4(0, 0, h, 0));
		vertices.push_back(topFace[i + 1] + Vec4(0, 0, h, 0));
	}
	for (int i = 0; i < size; i++) {
		vertices.push_back(topFace[i]);
		vertices.push_back(topFace[(i + 1) % size]);
		vertices.push_back(topFace[(i + 1) % size] + Vec4(0, 0, h, 0));

		vertices.push_back(topFace[i]);
		vertices.push_back(topFace[(i + 1) % size] + Vec4(0, 0, h, 0));
		vertices.push_back(topFace[i] + Vec4(0, 0, h, 0));

	}
	indices.resize(vertices.size());
	for (int i = 0; i < indices.size(); i++)
		indices[i] = i;

	return new M(vertices, indices, color);
}

template<typename M>
Model* ModelManager<M>::getTriangularPrism(Vec4 color) {
	vector<Vec4> topFace = { lb,rb,lt };
	return getPrism(topFace, color);
}
template<typename M>
Model* ModelManager<M>::getSquarePrism(Vec4 color) {
	vector<Vec4> topFace = { lb, rb, rt, lt };
	return getPrism(topFace, color);
}
template<typename M>
Model* ModelManager<M>::getParallelogramPrism(Vec4 color) {
	vector<Vec4> topFace = { lb, rb, Vec4(1.5f, 0.5f, 0.0f, 1.0f), Vec4(0.5f, 0.5f, 0.0f, 1.0f) };
	return getPrism(topFace, color);
}

typedef struct {
	GLint v_index[3];
	GLint t_index[3];
	GLint n_index[3];
} Face;

template<typename M>
Model* ModelManager<M>::loadObj(string path) {
	vector<Vec3> vertices;
	vector<Vec3> normals;
	vector<Vec2> uvs;
	vector<Face> faces;

	int l = (int)path.find_last_of("/");
	string name = path.substr(l + 1, string::npos);
	//string texture_path = path + "/" + name + ".png";
	//string obj_path = path + "/" + name + ".obj";

	FILE *file = fopen(path.c_str(), "r");
	
	if (!file) {
		printf("OBJ file \"%s\" not found\n", path.c_str());
		return NULL;
	}

	int tmp = 1;
	char buffer[1000];
	char line[256];
	while (fscanf(file, "%s", line) != EOF) {
		Vec3 coord;
		Vec2 coord2;
		Face face;
		if (!strcmp(line, "v")) {
			tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			vertices.push_back(coord);
		}
		else if (!strcmp(line, "vn")) {
			tmp = fscanf(file, "%f %f %f", &coord.x, &coord.y, &coord.z);
			normals.push_back(coord);
		}
		else if (!strcmp(line, "vt")) {
			tmp = fscanf(file, "%f %f", &coord2.x, &coord2.y);
			uvs.push_back(coord2);
		}
		else if (!strcmp(line, "f")) {
			tmp = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &face.v_index[0], &face.t_index[0], &face.n_index[0],
				&face.v_index[1], &face.t_index[1], &face.n_index[1], &face.v_index[2], &face.t_index[2], &face.n_index[2]);

			faces.push_back(face);
		}
		else if (!strcmp(line, "#")) {
			fgets(buffer, 1000, file);
		}
		else if (!strcmp(line, "g")) {
			fgets(buffer, 1000, file);
		}
	}

	vector<Vec4> out_vertices, out_normals;
	vector<Vec2> out_uvs;
	vector<GLubyte> out_elements;
	out_elements.resize(faces.size() * 3);
	for (GLubyte i = 0; i < faces.size(); i++) {
		out_elements.push_back(i * 3);
		out_elements.push_back(i * 3 + 1);
		out_elements.push_back(i * 3 + 2);
		
		out_vertices.push_back(vertices[faces[i].v_index[0] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[1] - 1]);
		out_vertices.push_back(vertices[faces[i].v_index[2] - 1]);

		out_normals.push_back(normals[faces[i].n_index[0] - 1]);
		out_normals.push_back(normals[faces[i].n_index[1] - 1]);
		out_normals.push_back(normals[faces[i].n_index[2] - 1]);

		out_uvs.push_back(uvs[faces[i].t_index[0] - 1]);
		out_uvs.push_back(uvs[faces[i].t_index[1] - 1]);
		out_uvs.push_back(uvs[faces[i].t_index[2] - 1]);

	}

	fclose(file);

	Model* model = new M(out_vertices, out_elements, Vec4(1, 0, 0, 1));
	return model;
}

const string MODEL_DIR = "res/meshes/";

template<typename M>
Model* ModelManager<M>::getObj(string name) {
	if (ModelManager::models.find(name) != ModelManager::models.end())
		return ModelManager::models[name];
	else {
		M* model = ModelManager::loadObj(MODEL_DIR + name + ".obj");
		return ModelManager::models[name] = model;
	}
}

