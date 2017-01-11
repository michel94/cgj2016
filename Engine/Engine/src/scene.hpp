#pragma once
#include "scenenode.hpp"
#include "camera.hpp"
#include "light.hpp"

class SceneNode;
class Camera;

class Scene {
public:
	Scene();
	~Scene();
	void render();
	void update(float dt);
	SceneNode* root();
	void addLight(Light* light);
	void removeLight(Light* light);
	void attachCamera(Camera* camera);
	Camera* getCamera();
private:
	Camera* camera;
	SceneNode* mRoot;
	float* pos;
	vector<Light*> lights;
};

