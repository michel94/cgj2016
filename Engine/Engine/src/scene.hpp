#pragma once
#include "scenenode.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include <unordered_map>

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
	void putSkybox(string name, GLuint id);
	GLuint getSkybox(string name);
	
	Camera* getCamera();
	vector<Light*> lights;


private:
	Camera* camera;
	SceneNode* mRoot;
	float* pos;
	unordered_map<string, GLuint> skybox;

};

