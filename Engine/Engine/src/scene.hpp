#pragma once

#include "scenenode.hpp"
#include "camera.hpp"

class SceneNode;

class Scene {
public:
	Scene();
	void render();
	void update(float dt);
	SceneNode* root();
	void attachCamera(Camera* camera);
private:
	Camera* camera;
	SceneNode* mRoot;
};
