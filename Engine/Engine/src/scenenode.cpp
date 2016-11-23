#include "scenenode.hpp"

SceneNode::SceneNode() {
	this->model = NULL;
	parent = NULL;
}

SceneNode::SceneNode(Model* model){
	this->model = model;
	parent = NULL;
}

SceneNode::SceneNode(SceneNode* parent) {
	model = NULL;
	this->parent = parent;
}

SceneNode::SceneNode(Model* model, SceneNode * parent){
	this->model = model;
	this->parent = parent;
}

SceneNode::~SceneNode(){
	for (auto n : children)
		delete n;
}

void SceneNode::setModelMatrix(Mat4 m) {
	mat = m; // No longer, can be renable by extending class and changing calcModelMatrix method
}

Mat4 SceneNode::calcModelMatrix() {
	return Mat4::translate(position) * Mat4::scale(scale) * rotation.toMat4();
}

Mat4 SceneNode::getModelMatrix() {
	return calcModelMatrix();
}

void SceneNode::addChild(SceneNode * node){
	children.push_back(node);
}

void SceneNode::addChildren(vector<SceneNode*>& nodes){
	int size = children.size() + (nodes.end() - nodes.begin());
	children.reserve(size);

	children.insert(children.end(), nodes.begin(), nodes.end());
}

bool SceneNode::removeChild(SceneNode * node){
	for (int i = 0; i < children.size(); i++) {
		if (children[i] == node) {
			swap(children[i], children[children.size() - 1]);
			children.pop_back();
			return true;
		}
	}
	return false;
}

void SceneNode::removeChildren(){
	children.clear();
}

void SceneNode::setParent(SceneNode * p){
	p->addChild(this);
	this->parent = p;
}

void SceneNode::switchParent(SceneNode * p){
	if (parent != NULL)
		parent->removeChild(this);
	parent = p;
	parent->addChild(this);
}

void SceneNode::update(float dt){
	// SceneNodes with animations need to extend this function
}

void SceneNode::updateChildren(float dt) {
	for (auto c : children) {
		c->update(dt);
		c->updateChildren(dt);
	}
}

void SceneNode::renderChildren(Mat4 tr) {
	for (auto c : children) {
		c->render(tr * c->getModelMatrix());
		c->renderChildren(tr * c->getModelMatrix());
	}
}

void SceneNode::render(Mat4 tr){
	if (model) {
		
		
		Shader& shader = ShaderManager::getShader("colored");
		shader.bind();
		
		glUniformMatrix4fv(shader["Matrix"], 1, GL_TRUE, tr.data);
		model->draw(shader);

		glUseProgram(0);
		glBindVertexArray(0);
	}
	
}

ColoredNode::ColoredNode(Model* model, Vec4 color) : SceneNode(model) {
	this->color = color;
}

void ColoredNode::render(Mat4 tr) {
	if (model) {

		Shader& shader = ShaderManager::getShader("colored");
		shader.bind();

		glUniformMatrix4fv(shader["Matrix"], 1, GL_TRUE, tr.data);
		glUniform4fv(shader["Color"], 1, (float*)color.data());
		model->draw(shader);

		glUseProgram(0);
		glBindVertexArray(0);
	}

}