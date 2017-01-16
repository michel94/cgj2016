#include "RainParticleSystem.hpp"

#define GRAVITY 9.8

RainParticleSystem::RainParticleSystem(int nParticles, float floor, float roof) : ParticleSystem(nParticles) {
	shader = ShaderManager::instance().getShader("rain");
	particleSize = Vec2(0.005f, 0.1f);
	this->floor = floor;
	this->roof = roof;

	sortParticles = true;
}

void RainParticleSystem::updateParticle(Particle* particle, float dt) {
	particle->position += particle->speed * dt;
}

Particle* RainParticleSystem::createParticle(float& timeSinceLast) {
	if (step == 0 || timeSinceLast < step) {
		return NULL;
	}
	timeSinceLast -= step;
	//Create particle
	float randomX = fRand(-5.0f, 5.0f);
	float randomZ = fRand(-5.0f, 5.0f);
	float speed = -0.5 * GRAVITY * fRand(0.5, 2.0);
	
	Particle* particle = new Particle(
		Vec3(randomX, this->roof, randomZ),
		speed * Vec3(0.0f, 1.0f, 0.0f),
		Vec4(0.5f, 0.7f, 1.0f, 0.2f),
		(roof - floor) / -speed,
		Vec2(0.005f, (float)fRand(0.1f, 0.5f))
	);
	return particle;

}

void RainParticleSystem::setIntensity(float i) {
	step = (stepMax - stepMin) * i + stepMin;
	cout << "Step " << step << endl;
}
