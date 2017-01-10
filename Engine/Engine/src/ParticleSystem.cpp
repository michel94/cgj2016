#include "ParticleSystem.hpp"


ParticleSystem::ParticleSystem(SceneNode * parent) : SceneNode(parent) {
	this->particles = vector<Particle*>();
	//This creates 
	shader = ShaderManager::instance().getShader("rain");
	std::cout << "ParticleSystem ctor";
	//createParticles(particles);
}

void ParticleSystem::update(float dt) {
	update(particles, dt);
	for (size_t i = 0; i < particles.size(); i++)
	{
		children[i]->position = particles[i]->position;
	}
}


