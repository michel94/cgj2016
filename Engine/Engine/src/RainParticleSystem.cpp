#include "RainParticleSystem.hpp"

#define GRAVITY 9.8

RainParticleSystem::RainParticleSystem(int nParticles, float floor, float roof) : ParticleSystem(nParticles) {
	//shader = ShaderManager::instance().getShader("rain");
	particleSize = Vec2(0.005f, 0.02f);
	this->floor = floor;
	this->roof = roof;
}

void RainParticleSystem::updateParticle(Particle* particle, float dt) {
	if (particle->position.y <= floor) {
		particle->position.y += (roof - floor);
	}
	particle->position += particle->speed * dt;
}

Particle* RainParticleSystem::createParticle(float& timeSinceLast) {
	if (!initialized) {
		timeSinceLast -= 0;
		if (particlesCount >= MAX_PARTICLES) {
			initialized = true;
			return NULL;
		}
		particlesCount++;

		//Create particle
		float randomX = fRand(-2.0f, 2.0f);
		float randomY = fRand(floor, roof);
		float randomZ = fRand(-2.0f, 2.0f);
		Particle* particle = new Particle(
			Vec3(randomX, randomY, randomZ),
			-0.5 * GRAVITY * Vec3(0.0f, 1.0f, 0.0f),
			Vec4(0.0f, 0.0f, 1.0f, 0.6f),
			100000.0f,
			particleSize
		);
		return particle;
	}
	return NULL;
}
