#include <cstdlib>
#include <cmath>
#include <limits>
#include <algorithm>

#include "fire.hpp"
#include "scene.hpp"

Vec2 randVec2(double radius) {
	double angle = fRand(0, 2 * 3.14159);
	return gaussianRandom(0, radius) * Vec2(cos(angle), sin(angle));
}

Vec2 uniformVec2(double radius) {
	double angle = fRand(0, 2 * 3.14159);
	return fRand(0, radius) * Vec2(cos(angle), sin(angle));
}

FireTarget::FireTarget(Vec3 position, Vec3 speed, Vec3 dest, float forceIntensity) {
	this->position = position;
	this->speed = speed;
	this->dest = dest;
	this->forceIntensity = forceIntensity;
}

void FireTarget::update(float dt) {
	Vec3 force = dest - position;
	force.normalized();
	force *= forceIntensity;
	speed += force * dt;
	Vec3 npos = position + speed * dt;
	if ((npos - dest).norm() < 0.25)
		position = npos;

}

Fire::Fire(SceneNode* parent, int nParticles, int nTargets, float height) : ParticleSystem(nParticles) {
	h = height;
	dest = source + Vec3(0, h, 0);
	for (int i = 0; i < nTargets; i++) {
		Vec2 randPos = uniformVec2(0.01);
		targets.push_back(new FireTarget(
			dest + Vec3(randPos.x, fRand(-0.01, 0.01), randPos.y),
			Vec3(0, 0, 0),
			dest,
			1 / h + i / 5.0f
			));
	}
	life = h + h / 5;
	this->parent = parent;	


	Scene* scene = getScene();
	Vec4 color = Vec4(1.0f, 0.5f, 0.0f, 1.0f);

	lights.resize(LIGHT_SPOTS);
	particlesCount.resize(LIGHT_SPOTS);
	for (int i = 0; i < lights.size(); i++) {
		lights[i] = new PointLight(position + Vec3( (i / 2) * 0.5f - 0.25f, 0.4, (i % 2) * 0.5f - 0.25f), color);
		scene->addLight(lights[i]);
	}

}

Particle* Fire::createParticle(float& timeSinceLast) {
	float step = 0.00001;
	if (timeSinceLast > step) {
		timeSinceLast -= step;
		Vec2 randPos = randVec2(0.05);
		Vec2 randVel = randVec2(0.6);
		while(randVel.norm() > 0.6)
			randVel = randVec2(0.6);
		
		float maxSpeed = 0.8;
		Vec3 speed = Vec3(randVel.x, fRand(0, maxSpeed), randVel.y);
		return new FireParticle(
			source + Vec3(randPos.x, fRand(0, 0.1), randPos.y),
			speed,
			Vec4(1, 0, 0, 1),
			life + fRand(0, h / 5.0f),
			particleSize,
			targets[rand() % targets.size()]
		);
	}
	else
		return NULL;
}

void Fire::updateParticle(Particle * p, float dt) {
	FireParticle* particle = (FireParticle*)p;
	Vec3 force = particle->target->position - particle->position;
	force.normalized();
	force *= 7;
	Vec3 sp = particle->speed.normalize(), f = force.normalize();
	if (particle->position.y < particle->target->position.y) {
		if (sp.dot(f) < 0.8) {
			particle->speed += force * dt;
		}
		else {
			force = particle->speed.normalize() * 2;
			particle->speed += force * dt;
		}
	}
	particle->position += particle->speed * dt;
	particlesCount[ (int)(particle->position.x < 0) * 2 + (int)(particle->position.z < 0) ]++;

	float age = particle->age / particle->life;
	float fireLimit = 0.8;
	if (age < fireLimit) {
		float intens = 1 - (age / fireLimit);
		float y = particle->position.y / (10 * h);
		float radius = particle->position.x * particle->position.x + y*y + particle->position.z * particle->position.z;
		radius = sqrt(radius);
		float v = max(0.0f, 0.5f - radius);
		Vec4 centerColor = Vec4(0, v, 0, 0);
		particle->color = Vec4(0.8f, intens, 0.05f, 0.6f) + centerColor;

	}else {
		particle->size = particleSize * 0.5f;
		float v = (age - fireLimit) / (1 - fireLimit) + 0.5;
		particle->color = Vec4(0.5f, 0.5f, 0.5f, 0.4f);
	}

}

void Fire::update(float dt) {
	std::fill(particlesCount.begin(), particlesCount.end(), 0);
	ParticleSystem::update(dt);

	for (int i = 0; i < targets.size(); i++) {
		targets[i]->update(dt);
	}
	for (int i = 0; i < particlesCount.size(); i++) {
		if (particles.size() > 0) {
			lights[i]->color = Vec4(1.0f, 0.5f, 0.0f, 1.0f) * ((float)particlesCount[i] / particles.size());
		}	
	}	
}

void Fire::setPosition(Vec3 pos) {
	position = pos;
	for (int i = 0; i < lights.size(); i++) {
		lights[i]->position = position + Vec3((i / 2) * 0.5f - 0.25f, 0.4, (i % 2) * 0.5f - 0.25f);
	}
}