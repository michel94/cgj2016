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
	/*rainTimer += dt;
	if (rainTimer > 10) {
		rainTimer = 0;
		rainStepIdx = ++rainStepIdx % rainStepSize;
		rain->step = rainSteps[rainStepIdx];
		cout << "Step " << rain->step << endl;
	}*/

	if (rainActive) {
		elapsedTime += dt;
		if (elapsedTime < duration) {
			cout << elapsedTime << " " << rain->step << endl;
			particlesPerSecond = elapsedTime / duration * finalPPS;
			if (particlesPerSecond != 0)
				rain->step = 1.0f / particlesPerSecond;
			else
				rain->step = 1 / 10;
		}
		else {
			cout << elapsedTime << " " << rain->step << endl;
			if (elapsedTime >= duration * 2)
				rainActive = false;
			particlesPerSecond = (2 * duration - elapsedTime) / duration * finalPPS;
			if (particlesPerSecond != 0)
				rain->step = 1.0f / particlesPerSecond;
			else
				rain->step = 1 / 10;
		}
	}
	
	
}