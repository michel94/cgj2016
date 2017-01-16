#include "DayNightCycle.hpp"

DayNightCycle::DayNightCycle(RainParticleSystem* rain, Fire* fire, SkyBoxNode* skybox, SceneNode* water, float dayDuration)
{
	this->rain = rain;
	this->fire = fire;
	this->skybox = skybox;
	this->water = water;
	this->dayDuration = skybox->dayDuration = dayDuration;
}

void DayNightCycle::update(float dt) {
	updateRain(dt);
	updateWater(dt);
}

void DayNightCycle::startRain() {
	rainActive = true;
	elapsedTime = 0;
}

void DayNightCycle::updateWater(float dt) {
	if (rainActive) {
		water->position.y +=  0.003 * dt * particlesPerSecond / finalPPS;
	}
	else {
		if(water->position.y < -4){
			float step = 0.003;
			//float step = 0.03;
			water->position.y -= dt * step;
		}
	}
}

void DayNightCycle::updateRain(float dt) {
	if (!rainActive) {
		double prob = ((double)dt / dayDuration) * rainProb;
		if (fRand(0.0, 1.0) < prob)
			startRain();
	}

	if (rainActive) {
		elapsedTime += dt;
		if (elapsedTime < duration) {
			cout << elapsedTime << " " << rain->step << endl;
			particlesPerSecond = elapsedTime / duration * finalPPS;
			if (particlesPerSecond != 0)
				rain->step = 1.0f / particlesPerSecond;
			else
				rain->step = 10.0f;
		}
		else {
			cout << elapsedTime << " " << rain->step << endl;
			if (elapsedTime >= duration * 2) {
				rain->step = 2000.0f;
				rainActive = false;
			}else {
				particlesPerSecond = (2 * duration - elapsedTime) / duration * finalPPS;
				if (particlesPerSecond != 0)
					rain->step = 1.0f / particlesPerSecond;
				else
					rain->step = 10.0f;
			}
			
		}
	}
	
	
}