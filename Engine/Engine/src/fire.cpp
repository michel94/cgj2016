#include <cstdlib>
#include <cmath>
#include <limits>

#include "fire.hpp"

Vec2 randVec2(double radius) {
	double angle = fRand(0, 2 * 3.14159);
	//return fRand(0, radius) * Vec2(cos(angle), sin(angle));
	return gaussianRandom(0, radius) * Vec2(cos(angle), sin(angle));
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

Fire::Fire(int nParticles, int nTargets, float height) : ParticleSystem(nParticles) {
	h = height;
	dest = source + Vec3(0, h, 0);
	float a = h / 5;
	for (int i = 0; i < nTargets; i++) {
		targets.push_back(new FireTarget(
			dest + Vec3(fRand(a, -a), fRand(a / 2, -a / 2), fRand(a, -a)),
			Vec3(fRand(-h / 2, h / 2), 0, fRand(-h / 2, h / 2)),
			dest,
			1 + i / 2.0f
			));
	}
	life = h;
}

Particle* Fire::createParticle(float& timeSinceLast) {
	float step = 0.00005;
	if (timeSinceLast > step) {
		timeSinceLast -= step;
		Vec2 randPos = randVec2(0.01);
		Vec2 randVel = randVec2(0.5);
		return new FireParticle(
			source + Vec3(randPos.x, 0, randPos.y),
			Vec3(randVel.x, fRand(0, 0.8), randVel.y),
			Vec4(1, 0, 0, 1),
			life + fRand(0, h / 5.0f),
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
	force *= 5;
	if (particle->position.y < particle->target->position.y) {
		Vec3 sp = particle->speed.normalize(), f = force.normalize();
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
	float fireLimit = 0.7;
	if (age < fireLimit)
		particle->color = Vec4(1, 1 - (age / fireLimit), 0, 0.4f);
	else {
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
