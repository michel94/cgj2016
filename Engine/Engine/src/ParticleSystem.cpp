#include "ParticleSystem.hpp"

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}



ParticleSystem::ParticleSystem(SceneNode * parent, int particles, int lifetime) : SceneNode(parent) {
	particles_ = particles;
	lifetime_ = lifetime;
	shader = ShaderManager::instance().getShader("rain");
	std::cout << "ctor";
	createParticles(particles);
}

void ParticleSystem::createParticles(int particles) {
	Mesh* m = ModelManager::instance().getObj("plane");
	Shader* shader = ShaderManager::instance().getShader("rain");

	for (size_t i = 0; i < particles; i++)
	{
		SceneNode* droplet = new SceneNode(m, this);
		float randomX = fRand(-2.0f, 2.0f);
		float randomY = fRand(-5.0f, 5.0f);
		float randomZ = fRand(-2.0f, 2.0f);
		droplet->position = Vec3(randomX, randomY, randomZ);
		droplet->scale.x = 0.005;
		droplet->scale.y = fRand(0.01, 0.03);
		droplet->scale.z = 0;

		droplet->setShader(shader);
		addChild(droplet);
	}
}

void ParticleSystem::update(float dt) {
	for (size_t i = 0; i < particles_; i++)
	{
		if (children[i]->position.y < -4) {
			children[i]->position.y = 5;
		}
		children[i]->position.y -= fRand(0.04, 0.05);
		
	}

}


