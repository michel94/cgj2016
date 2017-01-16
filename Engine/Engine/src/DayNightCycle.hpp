#pragma once

#include "RainParticleSystem.hpp"
#include "fire.hpp"
#include "skyboxnode.hpp"

class DayNightCycle {
public:
	DayNightCycle(RainParticleSystem*, Fire*, SkyBoxNode*, float);
	void update(float dt);

private:
	RainParticleSystem* rain;
	Fire* fire;
	SkyBoxNode* skybox;

	float dayDuration;
	/*
	float rainTimer;
	float dayNightTimer;
	const float dayNightCycleTime = 30; //secs
	unsigned int rainStepIdx;
	const float rainSteps[9] = { 0.0f, 0.1f, 0.01f, 0.001f, 0.0005f, 0.001f, 0.01f, 0.1f, 0.0f };
	const int rainStepSize = 9;*/

	void startRain();
	void updateRain(float);

	float rainProb = 2.0;
	float particlesPerSecond = 0;
	float duration = 20;
	float finalPPS = 1000;
	float elapsedTime = 0.0f;
	bool rainActive = false;
};