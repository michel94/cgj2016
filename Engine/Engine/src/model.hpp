#pragma once

#include <GL/glew.h>
#include <vector>
#include "vec.hpp"
#include "shaders.hpp"

using namespace std;

class Model;
typedef Model Mesh;

typedef struct {
	GLfloat x, y, z;
} Vertex;

typedef struct {
	GLfloat u, v;
} Texcoord;

typedef struct {
	GLfloat nx, ny, nz;
} Normal;

class Model {
public:
	Model();
	~Model();
	virtual void draw();
	virtual void createBuffers();
	
	const static GLuint VERTICES = 0;
	const static GLuint TEXCOORDS = 2;
	const static GLuint NORMALS = 3;
	const static GLuint TANGENTS = 4;
	
	std::vector<Vertex> Vertices;
	std::vector<Texcoord> Texcoords;
	std::vector<Normal> Normals;
	std::vector<Vertex> Tangents;
	bool TexcoordsLoaded=false, NormalsLoaded=false, TangentsLoaded=false;

	GLuint vao_id;
	GLuint vbo_vertices_id;
	GLuint vbo_normals_id;
	GLuint vbo_texcoords_id;
	GLuint vbo_tangents_id;
	Vec4 color;
	
	std::vector<Vertex> vertexData;
	std::vector<Texcoord> texcoordData;
	std::vector<Normal> normalData;
	std::vector<Vertex> tangentsData;
	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;
protected:


};

