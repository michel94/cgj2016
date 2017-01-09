#pragma once

#include "ParticleSystem.hpp"

class RainParticleSystem : public ParticleSystem {
public:
	RainParticleSystem(SceneNode* parent);
protected:
	void update(vector<Particle*>& particles, float dt);
private:
	void RainParticleSystem::createParticles();
};