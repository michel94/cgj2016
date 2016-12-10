#include "scene.hpp"

Scene::Scene() {
	mRoot = new SceneNode();
}

Scene::~Scene() {
	delete mRoot;
	if(camera)
		delete camera;

	ShaderManager::instance().getShader("CameraBlock");
}

void Scene::update(float dt) {
	mRoot->update(dt);
}

void Scene::render() {
	Block* block = ShaderManager::instance().getUniformBlock("CameraBlock");
	block->bind();
	Mat4 proj = camera->getProjection();
	Mat4 view = camera->getView();
	view.transpose();
	block->putMat4("ProjMatrix", proj);
	block->putMat4("ViewMatrix", view);
	
	Mat4 VP;

	mRoot->renderChildren(VP);
}

SceneNode* Scene::root() {
	return mRoot;
}

void Scene::attachCamera(Camera* camera) {
	this->camera = camera;
}

