#pragma once
#include "scenenode.hpp"

class MaterialNode : public SceneNode {
public:
	MaterialNode(Model * model, string name);
	MaterialNode(Model * model, SceneNode * parent, string name);
	void render(Mat4 tr);
	float reflectionBlend = 0.3;
protected:
	Texture* normalTexture;
};

