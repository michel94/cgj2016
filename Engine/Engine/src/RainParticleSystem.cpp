#include "RainParticleSystem.hpp"
#include "utils.hpp"

#define MAX_PARTICLES 100

RainParticleSystem::RainParticleSystem(SceneNode* parent) : ParticleSystem(parent) {
	createParticles();
}

void RainParticleSystem::createParticles() {
	Mesh* m = ModelManager::instance().getObj("plane");
	Shader* shader = ShaderManager::instance().getShader("rain");

	for (size_t i = 0; i < MAX_PARTICLES; i++)
	{
		//Create particle
		Particle * particle = new Particle();
		float randomX = fRand(-2.0f, 2.0f);
		float randomY = fRand(-5.0f, 5.0f);
		float randomZ = fRand(-2.0f, 2.0f);
		particle->position = Vec3(randomX, randomY, randomZ);
		particles.push_back(particle);

		//Create scenenode
		SceneNode* droplet = new SceneNode(m, this);
		droplet->scale.x = 0.20;
		droplet->scale.y = 0.20;
		droplet->scale.z = 0;
		droplet->setShader(shader);
		addChild(droplet);
	}
}

#define GRAVITY 9.8

void RainParticleSystem::update(vector<Particle*>& particles, float dt) {
	for (size_t i = 0; i < particles.size(); i++)
	{
		Particle * particle = particles[i];
		if (particle->position.y < -4) {
			particle->position.y = 5;
		}
		particle->position.y -= 0.5 * GRAVITY * (dt);

	}
}


