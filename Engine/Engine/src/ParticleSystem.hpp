#pragma once
#include "scenenode.hpp"
#include "ModelManager.hpp"

class ParticleSystem : public SceneNode {
public:
	ParticleSystem(SceneNode * parent, int particles, int lifetime);
	void update(float dt);
private:
	int particles_;
	int lifetime_;
};

