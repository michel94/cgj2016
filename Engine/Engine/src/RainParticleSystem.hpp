#pragma once

#include "ParticleSystem.hpp"

class RainParticleSystem : public ParticleSystem {
	const float stepMax = 0.0005;
	const float stepMin = 0.05;

public:
	RainParticleSystem(int nParticles, float floor, float roof);

	void setIntensity(float i);

protected:
	virtual void updateParticle(Particle* particle, float dt);
	virtual Particle* RainParticleSystem::createParticle(float& timeSinceLast);

private:
	float floor, roof;

	bool initialized = false;
	int particlesCount = 0;
	float step = (stepMax + stepMin) / 2;

};