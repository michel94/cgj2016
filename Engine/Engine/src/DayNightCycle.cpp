#include "DayNightCycle.hpp"

DayNightCycle::DayNightCycle(RainParticleSystem* rain, Fire* fire, SkyBoxNode* skybox, SceneNode* water, Light* light, float dayDuration)
{
	this->rain = rain;
	this->fire = fire;
	this->skybox = skybox;
	this->water = water;
	this->dayDuration = skybox->dayDuration = dayDuration;
	this->light = light;
	this->sunColor = light->color;
}

void DayNightCycle::update(float dt) {
	updateRain(dt);
	updateWater(dt);
	updateLight(dt);
}

void DayNightCycle::startRain() {
	rainActive = true;
	elapsedTime = 0;
}

void DayNightCycle::updateWater(float dt) {
	if (rainActive) {
		water->position.y +=  0.005 * dt * particlesPerSecond / finalPPS;
	}
	else {
		if(water->position.y > -5.4){ //The water is visible
			water->position.y -= dt * 0.005;
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
			particlesPerSecond = elapsedTime / duration * finalPPS;
			if (particlesPerSecond != 0)
				rain->step = 1.0f / particlesPerSecond;
			else
				rain->step = 10.0f;
		}
		else {
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

Vec4 mix(Vec4 a, Vec4 b, float d){
	if (d < 0)
		d = 0;
	else if (d > 1)
		d = 1;
	return b*d + a*(1 - d);
	
}

void DayNightCycle::updateLight(float dt){
	float angle = (dt / dayDuration) * 180.0f;
	light->position = light->position * Mat4::rotateAround(Vec3(0.0f, 1.0f, 0.0f), angle);
	if (skybox->daynight==1.0f)
	{
		light->color = mix(sunColor, sunColor*0.7f, skybox->blendfactor);
	}
	else
	{
		light->color = mix(sunColor, sunColor*0.7f, skybox->blendfactor);
	}

}