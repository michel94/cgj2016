#pragma once

#include "ParticleSystem.hpp"

Vec2 randVec2(double radius);
double gaussianRandom(double mu, double sigma);

class FireTarget {
public:
	Vec3 position, speed;
	Vec3 dest;
	float forceIntensity;
	FireTarget(Vec3 position, Vec3 speed, Vec3 dest, float forceIntensity) {
		this->position = position;
		this->speed = speed;
		this->dest = dest;
		this->forceIntensity = forceIntensity;
	}
	void update(float dt) {
		Vec3 force = dest - position;
		force.normalized();
		force *= forceIntensity;
		speed += force * dt;
		position += speed * dt;
	}
};

class FireParticle : public Particle {
public:
	FireParticle(Vec3 position, Vec3 speed, Vec4 color, float life, FireTarget* target)
		: Particle(position, speed, color, life) {
		this->target = target;
	}

	FireTarget* target;

	static const float LIFE;
};

class Fire : public ParticleSystem {
public:
	Fire(SceneNode* parent, int nTargets, float height);
	virtual Particle* createParticle(float& timeSinceLast);
	virtual void updateParticle(Particle* particle, float dt);
	virtual void update(float dt);

	Vec3 source, dest;
	float h, life;
private:
	//vector<Vec3> targets, targetSpeed;
	vector<FireTarget*> targets;

};