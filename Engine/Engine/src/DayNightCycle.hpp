#pragma once

#include "RainParticleSystem.hpp"
#include "fire.hpp"
#include "skyboxnode.hpp"

class DayNightCycle {
public:
	DayNightCycle(RainParticleSystem*, Fire*, SkyBoxNode*, SceneNode*, Light*, float);
	void update(float dt);

private:
	RainParticleSystem* rain;
	Fire* fire;
	SkyBoxNode* skybox;
	SceneNode* water;
	Light* light;
	Vec4 sunColor;

	float dayDuration;

	void startRain();
	void updateRain(float);
	void updateWater(float);
	void updateLight(float);

	float rainProb = 4.7;
	float particlesPerSecond = 0;
	float duration = 20;
	float finalPPS = 10000;
	float elapsedTime = 0.0f;
	bool rainActive = false;
};