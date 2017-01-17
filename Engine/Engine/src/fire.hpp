#pragma once

#include "ParticleSystem.hpp"
#include "light.hpp"

Vec2 randVec2(double radius);
double gaussianRandom(double mu, double sigma);

class FireTarget {
public:
	Vec3 position, speed;
	Vec3 dest;
	float forceIntensity;
	FireTarget(Vec3 position, Vec3 speed, Vec3 dest, float forceIntensity);
	void update(float dt);
};

class FireParticle : public Particle {
public:
	FireParticle(Vec3 position, Vec3 speed, Vec4 color, float life, Vec2 size, FireTarget* target)
		: Particle(position, speed, color, life, size) {
		this->target = target;
		this->initialSize = size;
	}

	FireTarget* target;
	Vec2 initialSize;

	static const float LIFE;
};

class Fire : public ParticleSystem {
public:
	Fire(SceneNode* parent, int nParticles, int nTargets, float height);
	virtual Particle* createParticle(float& timeSinceLast);
	virtual void updateParticle(Particle* particle, float dt);
	virtual void update(float dt);
	void setPosition(Vec3 pos);

	Vec3 source, dest;
	float h, life;
private:
	vector<FireTarget*> targets;
	
	const int LIGHT_SPOTS = 4;
	vector<Light *> lights;
	vector<float> particlesCount;
};
