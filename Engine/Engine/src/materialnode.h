#pragma once
#include "scenenode.hpp"

class MaterialNode : public SceneNode {
public:
	MaterialNode(Model * model, string name);
	MaterialNode(Model * model, SceneNode * parent, string name);
	void render(Mat4 tr);
protected:
	Texture* normalTexture;
};

