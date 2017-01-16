#include "materialnode.h"
#include "scene.hpp"

MaterialNode::MaterialNode(Model* model, string name) : SceneNode(model) {
	texture = TextureManager::instance().getTexture(name + ".png");
	normalTexture = TextureManager::instance().getTexture(name + "_normal.png");
	shader = ShaderManager::instance().getShader("normalmapping");
}

MaterialNode::MaterialNode(Model * model, SceneNode * parent, string name) : SceneNode(model, parent) {
	texture = TextureManager::instance().getTexture(name + ".png");
	normalTexture = TextureManager::instance().getTexture(name + "_normal.png");
	shader = ShaderManager::instance().getShader("normalmapping");
}

void MaterialNode::render(Mat4 tr) {
	if (model && shader) {
		Shader& s = *shader;
		s.bind();
		
		if (texture) {
			glActiveTexture(GL_TEXTURE0);
			texture->bind();
			glUniform1i(s["tex"], 0);
		}
		if (normalTexture) {
			glActiveTexture(GL_TEXTURE1);
			normalTexture->bind();
			glUniform1i(s["normalTex"], 1);
		}
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, getScene()->getSkybox("day"));
		glUniform1i(s["cube_texture"], 2);
		
		glUniformMatrix4fv(s["Matrix"], 1, GL_TRUE, tr.data);
		model->draw();

		if (texture)
			texture->unbind();

		s.unbind();
		glBindVertexArray(0);
	}
}
