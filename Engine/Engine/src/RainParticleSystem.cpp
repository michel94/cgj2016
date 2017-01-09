#include "RainParticleSystem.hpp"

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

#define MAX_PARTICLES 200

RainParticleSystem::RainParticleSystem(SceneNode* parent) : ParticleSystem(parent) {
	createParticles();
}

void RainParticleSystem::createParticles() {
	Mesh* m = ModelManager::instance().getObj("plane");
	Shader* shader = ShaderManager::instance().getShader("rain");

	for (size_t i = 0; i < MAX_PARTICLES; i++)
	{
		Particle * particle = new Particle();
		float randomX = fRand(-2.0f, 2.0f);
		float randomY = fRand(-5.0f, 5.0f);
		float randomZ = fRand(-2.0f, 2.0f);
		particle->position = Vec3(randomX, randomY, randomZ);
		particles.push_back(particle);


		SceneNode* droplet = new SceneNode(m, this);
		droplet->scale.x = 0.005;
		droplet->scale.y = fRand(0.01, 0.03);
		droplet->scale.z = 0;
		droplet->setShader(shader);
		addChild(droplet);
	}
}

void RainParticleSystem::update(vector<Particle*>& particles, float dt) {
	for (size_t i = 0; i < particles.size(); i++)
	{
		if (particles[i]->position.y < -4) {
			particles[i]->position.y = 5;
		}
		particles[i]->position.y -= fRand(0.04, 0.05);

	}
}


