#pragma once
#include "scenenode.hpp"
#include "ModelManager.hpp"
#include "particle.h"

double fRand(double fMin, double fMax);

class DynamicModel : public Model {
public:
	DynamicModel(int);
	~DynamicModel();
	void createBuffers();

	int maxParticles;
	GLuint vbo_colors_id, vbo_psize_id;
	vector<Color> Colors;
	vector<float> Psize;
};

class ParticleSystem : public SceneNode {
public:
	ParticleSystem(int nParticles);
	virtual void update(float dt);
	virtual Particle* createParticle(float& timeSinceLast) = 0;
	virtual void updateParticle(Particle* particle, float dt) = 0;
	virtual void render(Mat4 tr);
	virtual void renderChildren(Mat4 tr);

	Vec2 particleSize = Vec2(0.003, 0.003);
private:
	
	float timeSinceLast=0;
	vector<Particle*> particles;

	struct Less {
		Less(Vec3& r) : ref(r) {}
		bool operator () (const Particle* a, const Particle* b) {
			return (a->position - ref).norm() > (b->position - ref).norm();
		}
		Vec3& ref;
	};

protected:
	int MAX_PARTICLES;
	bool sortParticles = false;
};

