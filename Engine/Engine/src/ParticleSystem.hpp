#pragma once
#include "scenenode.hpp"
#include "ModelManager.hpp"
#include "particle.h"

double fRand(double fMin, double fMax);

class ParticleSystem : public SceneNode {
public:
	ParticleSystem(SceneNode * parent);
	virtual void update(float dt);
	virtual Particle* createParticle(float& timeSinceLast) = 0;
	virtual void updateParticle(Particle* particle, float dt) = 0;
private:
	SceneNode* getParticleNode();

	float timeSinceLast=0;
	vector<Particle*> particles;

	struct Less {
		Less(Vec3& r) : ref(r) {}
		bool operator () (const SceneNode* a, const SceneNode* b) {
			return (a->position - ref).norm() > (b->position - ref).norm();
		}
		Vec3& ref;
	};
};

