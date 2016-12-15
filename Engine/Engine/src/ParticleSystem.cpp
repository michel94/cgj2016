#include "ParticleSystem.hpp"

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

void createParticles(SceneNode* node, int particles) {
	Mesh* m = ModelManager::instance().getObj("plane");
	Shader* shader = ShaderManager::instance().getShader("rain");

	for (size_t i = 0; i < particles; i++)
	{
		SceneNode* droplet = new SceneNode(m, node);
		float randomX = fRand(-5.0f, 5.0f);
		float randomY = fRand(-5.0f, 5.0f);
		droplet->position.x = randomX;
		droplet->position.y = randomY;
		droplet->scale.x = 0.005;
		droplet->scale.y = fRand(0.01, 0.03);
		droplet->scale.z = 0;

		droplet->setShader(shader);
		node->addChild(droplet);
	}
}


ParticleSystem::ParticleSystem(SceneNode * parent, int particles, int lifetime) : SceneNode(parent) {
	particles_ = particles;
	lifetime_ = lifetime;
	shader = ShaderManager::instance().getShader("rain");
	
	createParticles(this, particles);
}

void ParticleSystem::update(float dt) {
	for (size_t i = 0; i < particles_; i++)
	{
		if (children[i]->position.y < -5) {
			children[i]->position.y = 5;
		}
		children[i]->position.y -= fRand(0.04, 0.05);
		
	}

}

