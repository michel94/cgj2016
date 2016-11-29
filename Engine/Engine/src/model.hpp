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
	virtual void draw(Shader& shader);
	virtual void createBuffers();
	
	const static GLuint VERTICES = 0;
	const static GLuint TEXCOORDS = 1;
	const static GLuint NORMALS = 2;
	
	std::vector<Vertex> Vertices;
	std::vector<Texcoord> Texcoords;
	std::vector<Normal> Normals;
	bool TexcoordsLoaded=false, NormalsLoaded=false;

	GLuint vao_id;
	GLuint vbo_vertices_id;
	GLuint vbo_normals_id;
	GLuint vbo_texcoords_id;
	Vec4 color;
	
	std::vector<Vertex> vertexData;
	std::vector<Texcoord> texcoordData;
	std::vector<Normal> normalData;
	std::vector <unsigned int> vertexIdx, texcoordIdx, normalIdx;
protected:


};

