#include "model.hpp"

Model::Model(){
}

Model::~Model() {
	glBindVertexArray(vao_id);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(TEXCOORDS);
	glDisableVertexAttribArray(NORMALS);
	glDeleteBuffers(1, &vbo_vertices_id);
	glDeleteBuffers(1, &vbo_texcoords_id);
	glDeleteBuffers(1, &vbo_normals_id);
	glDeleteVertexArrays(1, &vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Model::createBuffers() {
	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	{
		glGenBuffers(1, &vbo_vertices_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_id);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	
		if (TexcoordsLoaded){
			glGenBuffers(1, &vbo_texcoords_id);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_texcoords_id);
			glBufferData(GL_ARRAY_BUFFER, Texcoords.size() * sizeof(Texcoord), &Texcoords[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TEXCOORDS);
			glVertexAttribPointer(TEXCOORDS, 2, GL_FLOAT, GL_FALSE, sizeof(Texcoord), 0);
		}
		if (NormalsLoaded){
			glGenBuffers(1, &vbo_normals_id);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_normals_id);
			glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(Normal), &Normals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(NORMALS);
			glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, sizeof(Normal), 0);
		}
		if (TangentsLoaded) {
			glGenBuffers(1, &vbo_tangents_id);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_tangents_id);
			glBufferData(GL_ARRAY_BUFFER, Tangents.size() * sizeof(Vertex), &Tangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(TANGENTS);
			glVertexAttribPointer(TANGENTS, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

			glGenBuffers(1, &vbo_bitangents_id);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_bitangents_id);
			glBufferData(GL_ARRAY_BUFFER, Bitangents.size() * sizeof(Vertex), &Bitangents[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(BITANGENTS);
			glVertexAttribPointer(BITANGENTS, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		}
	}
	

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Model::draw() {
	glBindVertexArray(vao_id);
	glDrawArrays(GL_TRIANGLES, 0, (int)Vertices.size());
	glBindVertexArray(0);
}
