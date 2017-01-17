#pragma once

#include "RainParticleSystem.hpp"
#include "fire.hpp"
#include "skyboxnode.hpp"

class DayNightCycle {
public:
	DayNightCycle(RainParticleSystem*, Fire*, SkyBoxNode*, SceneNode*, float);
	void update(float dt);

private:
	RainParticleSystem* rain;
	Fire* fire;
	SkyBoxNode* skybox;
	SceneNode* water;

	float dayDuration;

	void startRain();
	void updateRain(float);
	void updateWater(float);

	float rainProb = 2.7;
	float particlesPerSecond = 0;
	float duration = 20;
	float finalPPS = 2000;
	float elapsedTime = 0.0f;
	bool rainActive = false;
};