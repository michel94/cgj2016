#pragma once
#include "scenenode.hpp"
#include "ModelManager.hpp"
#include "particle.h"

class ParticleSystem : public SceneNode {
public:
	ParticleSystem(SceneNode * parent);
	virtual void update(float dt);
protected:
	virtual void update(vector<Particle*>& particles, float dt) = 0;
	vector<Particle*> particles;
private:
	
	
};

