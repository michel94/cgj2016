#pragma once

#include "scenenode.hpp"
#include "SOIL.h"

class SkyBoxNode : public SceneNode {
public:
	SkyBoxNode(Model * model, SceneNode * parent);
	void render(Mat4 tr);
	virtual void update(float dt);

	float dayDuration = 50;
private:
	const float ROTATE_SPEED = 1.0f;
	GLuint loadCubemap(string folder, vector<string> faces);
	void bindTextures(Shader& s);
	float time;
	float blendfactor = 0;
	float daynight = 1.0f;  //true = dia para a noite
	GLuint skyboxDayId, skyboxNightId;
};