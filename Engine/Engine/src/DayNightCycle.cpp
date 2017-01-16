#include "DayNightCycle.hpp"

DayNightCycle::DayNightCycle(RainParticleSystem* rain, Fire* fire, SkyBoxNode* skybox)
{
	this->rain = rain;
	this->fire = fire;
	this->skybox = skybox;
}

void DayNightCycle::update(float dt) {
	updateRain(dt);
}

void DayNightCycle::updateRain(float dt) {
	rainTimer += dt;
	if (rainTimer > 10) {
		rainTimer = 0;
		rainStepIdx = ++rainStepIdx % rainStepSize;
		rain->step = rainSteps[rainStepIdx];
		cout << "Step " << rain->step << endl;
	}
}