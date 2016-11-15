#include "scene.hpp"

Scene::Scene() {
	mRoot = new SceneNode();
}

void Scene::update(float dt) {
	mRoot->update(dt);
}

void Scene::render() {
	Mat4 VP = camera->toMat4();

	mRoot->renderChildren(VP);
}

SceneNode* Scene::root() {
	return mRoot;
}

void Scene::attachCamera(Camera* camera) {
	this->camera = camera;
}

