#pragma once

#include "RainParticleSystem.hpp"
#include "fire.hpp"
#include "skyboxnode.hpp"

class DayNightCycle {
public:
	DayNightCycle(RainParticleSystem*, Fire*, SkyBoxNode*);
	void update(float dt);

private:
	RainParticleSystem* rain;
	Fire* fire;
	SkyBoxNode* skybox;

	void updateRain(float);

	float rainTimer;
	float dayNightTimer;
	const float dayNightCycleTime = 30; //secs
	unsigned int rainStepIdx;
	const float rainSteps[9] = { 0.0f, 0.1f, 0.01f, 0.001f, 0.0005f, 0.001f, 0.01f, 0.1f, 0.0f };
	const int rainStepSize = 9;

};