#pragma once
#include "scenenode.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "DirectionalLight.hpp"
#include "PointLight.hpp"
#include <unordered_map>

class SceneNode;
class Camera;

struct SkyboxData
{
	float blendfactor;
	float rotation;
	GLuint dayTex, nightTex;
};

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
	void setSkybox(SkyboxData* data);
	SkyboxData* getSkybox();
	
	Camera* getCamera();
	vector<Light*> lights;


private:
	Camera* camera;
	SceneNode* mRoot;
	float* pos;
	SkyboxData* skybox = NULL;

};

