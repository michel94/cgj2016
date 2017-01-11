#include "ParticleSystem.hpp"

#include <algorithm>

#include "scene.hpp"

double fRand(double fMin, double fMax){
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

ParticleSystem::ParticleSystem(SceneNode * parent) : SceneNode(parent) {
	shader = ShaderManager::instance().getShader("colored");
}

SceneNode* ParticleSystem::getParticleNode() {
	Mesh* m = ModelManager::instance().getObj("plane");
	Shader* shader = ShaderManager::instance().getShader("colored");
	
	//SceneNode* droplet = new SceneNode(m, this);
	ColoredNode* droplet = new ColoredNode(m, Vec4(0, 0, 0, 1));
	droplet->parent = this;

	float randomX = fRand(-2.0f, 2.0f);
	float randomY = fRand(-5.0f, 5.0f);
	float randomZ = fRand(-2.0f, 2.0f);
	droplet->position = Vec3(randomX, randomY, randomZ);
	droplet->scale = Vec3(0.004, 0.004, 0.004);

	droplet->setShader(shader);

	return droplet;
}

void ParticleSystem::update(float dt) {
	Particle* p;
	cout << "\r" << "Particles: " << particles.size() << endl;
	while (true) {
		p = createParticle(timeSinceLast);
		if (p != NULL) {
			particles.push_back(p);
			children.push_back(getParticleNode());
		}
		else {
			timeSinceLast += dt;
			break;
		}
	}
	
	for (size_t i = 0; i < particles.size(); i++){
		updateParticle(particles[i], dt);
		particles[i]->age += dt;
		children[i]->position = particles[i]->position;
		((ColoredNode*)children[i])->color = particles[i]->color;
		if (particles[i]->age >= particles[i]->life) {
			delete particles[i];
			delete children[i];
			particles.erase(particles.begin() + i);
			children.erase(children.begin() + i);
			i--;
		}
	}
	
	sort(children.begin(), children.end(), Less(getScene()->getCamera()->position) );
}
