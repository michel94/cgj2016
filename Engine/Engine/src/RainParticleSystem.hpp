#pragma once

#include "ParticleSystem.hpp"

class RainParticleSystem : public ParticleSystem {
public:
	RainParticleSystem(int nParticles, float floor, float roof);

protected:
	virtual void updateParticle(Particle* particle, float dt);
	virtual Particle* RainParticleSystem::createParticle(float& timeSinceLast);

private:
	float floor, roof;

	bool initialized = false;
	int particlesCount = 0;
};