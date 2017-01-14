#include <cstdlib>
#include <cmath>
#include <limits>
#include <algorithm>

#include "fire.hpp"

Vec2 randVec2(double radius) {
	double angle = fRand(0, 2 * 3.14159);
	//return fRand(0, radius) * Vec2(cos(angle), sin(angle));
	return gaussianRandom(0, radius) * Vec2(cos(angle), sin(angle));
}

Vec2 uniformVec2(double radius) {
	double angle = fRand(0, 2 * 3.14159);
	return fRand(0, radius) * Vec2(cos(angle), sin(angle));
}

double gaussianRandom(double mu, double sigma){
	const double epsilon = std::numeric_limits<double>::min();
	const double two_pi = 2.0 * atan(1.0);

	static double z0, z1;
	static bool generate;
	generate = !generate;

	if (!generate)
		return z1 * sigma + mu;

	double u1, u2;
	do{
		u1 = rand() * (1.0 / RAND_MAX);
		u2 = rand() * (1.0 / RAND_MAX);
	} while (u1 <= epsilon);

	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return z0 * sigma + mu;
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
	if ((npos - dest).norm() < 0.2)
		position = npos;

}

Fire::Fire(int nParticles, int nTargets, float height) : ParticleSystem(nParticles) {
	h = height;
	dest = source + Vec3(0, h, 0);
	for (int i = 0; i < nTargets; i++) {
		Vec2 randPos = uniformVec2(0.01);
		targets.push_back(new FireTarget(
			dest + Vec3(randPos.x, fRand(-0.01, 0.01), randPos.y),
			Vec3(0,0,0),
			//Vec3(fRand(-h / 2, h / 2), 0, fRand(-h / 2, h / 2)),
			dest,
			1 + i / 5.0f
			));
		cout << targets[i]->position << endl;
	}
	life = h;
}

Particle* Fire::createParticle(float& timeSinceLast) {
	float step = 0.00005;
	if (timeSinceLast > step) {
		timeSinceLast -= step;
		Vec2 randPos = randVec2(0.02);
		Vec2 randVel = randVec2(0.5);
		while(randVel.norm() > 0.5)
			randVel = randVec2(0.5);
		
		float maxSpeed = 0.8;
		Vec3 speed = Vec3(randVel.x, fRand(0, maxSpeed), randVel.y);
		return new FireParticle(
			source + Vec3(randPos.x, fRand(0, 0.05), randPos.y),
			speed,
			Vec4(1, 0, 0, 1),
			life + fRand(0, h / 5.0f),
			particleSize * (speed.y / maxSpeed),
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
	force *= 6;
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

	float age = particle->age / particle->life;
	float fireLimit = 0.9;
	if (age < fireLimit) {
		float intens = 1 - (age / fireLimit);
		float y = particle->position.y / (10 * h);
		float radius = particle->position.x * particle->position.x + y*y + particle->position.z * particle->position.z;
		radius = sqrt(radius);
		float v = max(0.0f, 0.5f - radius);
		Vec4 centerColor = Vec4(0, v, 0, 0);
		particle->color = Vec4(1, intens, 0, 0.6f) + centerColor;

	}else {
		float v = (age - fireLimit) / (1 - fireLimit) + 0.5;
		particle->color = Vec4(0.5, 0.5, 0.5, 0.4f);
	}

}

void Fire::update(float dt) {
	ParticleSystem::update(dt);
	for (int i = 0; i < targets.size(); i++) {
		targets[i]->update(dt);
	}

}
