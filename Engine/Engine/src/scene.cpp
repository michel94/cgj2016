#include "scene.hpp"

Scene::Scene() {
	mRoot = new SceneNode();
}

Scene::~Scene() {
	delete mRoot;
	if(camera)
		delete camera;
	
}

void Scene::update(float dt) {
	mRoot->update(dt);
}

void Scene::render() {
	Block* block = ShaderManager::instance().getUniformBlock("CameraBlock");
	Mat4 proj = camera->getProjection();
	Mat4 view = camera->getView();
	view.transpose();
	block->putMat4("ProjMatrix", proj);
	block->putMat4("ViewMatrix", view);
	Vec3 pos = camera->position;
	cout << pos << endl;
	block->putData("CameraPosition", (GLubyte*)pos.data(), pos.size());

	Mat4 M;
	mRoot->renderChildren(M);
}

SceneNode* Scene::root() {
	return mRoot;
}

void Scene::attachCamera(Camera* camera) {
	this->camera = camera;
}

