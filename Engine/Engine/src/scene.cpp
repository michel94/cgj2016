#include "scene.hpp"

Scene::Scene() {
	mRoot = new SceneNode();
	mRoot->scene = this;
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
	Block* cmBlock = ShaderManager::instance().getUniformBlock("CameraBlock");
	Mat4 proj = camera->getProjection();
	Mat4 view = camera->getView();
	view.transpose();
	cmBlock->putMat4("ProjMatrix", proj);
	cmBlock->putMat4("ViewMatrix", view);
	Vec3 pos = camera->position;
	cmBlock->putVec("CameraPosition", &pos);

	Block* lBlock = ShaderManager::instance().getUniformBlock("LightBlock");
	int nLights = (int) lights.size();
	lBlock->putData("nLights", (GLubyte*)&nLights, sizeof(int));

	for (int i = 0; i < nLights; i++) { // TODO: offset between array elements is hardcoded
		int size = lights[i]->position.size();
		glBufferSubData(GL_UNIFORM_BUFFER, lBlock->getOffset("LightPosition") + 16*i, size, lights[i]->position.data());
		glBufferSubData(GL_UNIFORM_BUFFER, lBlock->getOffset("LightColor") + 16*i, size, lights[i]->color.data()); // size is the same because both are Vec3
	}

	Mat4 M;
	mRoot->renderChildren(M);
}

SceneNode* Scene::root() {
	return mRoot;
}

void Scene::addLight(Light * light){
	lights.push_back(light);
}

void Scene::removeLight(Light * light){
	for (int i = 0; i < lights.size(); i++) {
		if (lights[i] == light) {
			swap(lights[lights.size() - 1], lights[i]);
			lights.pop_back();
		}
	}
}

void Scene::attachCamera(Camera* camera) {
	this->camera = camera;
}

Camera * Scene::getCamera(){
	return camera;
}

void Scene::setSkybox(SkyboxData* data) {
	skybox = data;
}

SkyboxData* Scene::getSkybox() {
	return skybox;
}
