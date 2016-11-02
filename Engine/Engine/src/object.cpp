#include "object.hpp"

Object::Object(Shader *s, Model *m) : shader(*s), model(*m) {
}

Object::~Object() {
}

void Object::draw(Mat4 wm){
	model.bind();
	glUseProgram(shader.programId);
	wm *= mat;
	
	glUniformMatrix4fv(shader["Matrix"], 1, GL_TRUE, wm.data);
	//glUniform4fv(shader["Color"], 1, (float*)color.data() );
	model.draw();
	
	glUseProgram(0);
	glBindVertexArray(0);

}

void Object::setModelMatrix(Mat4& m) {
	this->mat = m;
}
