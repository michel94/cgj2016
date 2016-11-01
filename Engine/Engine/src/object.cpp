#include "object.hpp"

Object::Object(Shader *s, Model *m) : shader(*s), model(*m) {
}

Object::~Object() {
}

void Object::setColor(Vec4 color) {
	this->color = color;
}

void Object::draw(Mat4 m){
	model.bind();
	glUseProgram(shader.programId);
	glUniformMatrix4fv(shader["Matrix"], 1, GL_TRUE, m.data);
	glUniform4fv(shader["Color"], 1, (float*)color.data() );
	
	model.draw();
	
	glUseProgram(0);
	glBindVertexArray(0);

}
