#pragma once

#include "quaternion.hpp"
#include "vec.hpp"
#include "scenenode.hpp"

const float PI = 3.14159f;


class Animation;

class AnimController {
public:
	virtual void onFinish(Animation* anim, float dt) = 0;
};

class Animation {
public:
	AnimController* controller=NULL;
	virtual void update(float dt) = 0;
	virtual Animation* reverse() = 0;
	void setController(AnimController* controller) {
		this->controller = controller;
	}
protected:
	virtual void finish(float dt) {
		if (controller != NULL)
			controller->onFinish(this, dt);
	}
};

class AnimManager : public AnimController {
public:
	static AnimManager& getInstance()
	{
		static AnimManager instance; // Guaranteed to be destroyed.
									 // Instantiated on first use.
		return instance;
	}
	AnimManager() {

	}
	~AnimManager() {
		for (auto a : animations)
			delete a;
	}
	void start(Animation* anim) {
		animations.push_back(anim);
		anim->setController(this);
	}
	void start(vector<Animation*> animations) {
		for (auto a : animations) {
			this->animations.push_back(a);
			a->setController(this);
		}
		
	}
	void update(float dt) {
		for (int i = 0; i < (int)animations.size(); i++) {
			animations[i]->update(dt);
		}
	}
	virtual void onFinish(Animation* anim, float dt) {
		for (int i = 0; i < (int)animations.size(); i++) {
			if (animations[i] == anim) {
				delete anim;
				animations[i] = animations[animations.size() - 1];
				animations.pop_back();
			}
		}
	}

protected:
	vector<Animation*> animations;

public:
	AnimManager(AnimManager const&) = delete;
	void operator=(AnimManager const&) = delete;
};

class Translation : public Animation {
	public:
	Translation(SceneNode* node, Vec3 v) {
		this->node = node;
		this->v = v;
		duration = v.norm() * (1 / speed);
	}
	Translation(SceneNode* node, Vec3 v, float speed) {
		this->node = node;
		this->v = v;
		this->speed = speed;
		duration = v.norm() * (1 / speed);
	}
	virtual void update(float dt) {
		if (t / duration <= 1.0f) {
			t += dt;
			node->position += v * (dt / duration);
		}
		if (t / duration > 1.0f)
			finish(t / duration - 1.0f);

	}
	virtual Translation* reverse() {
		return new Translation(node, -v);
	}

	float speed = 0.5f;
private:
	float duration, t;
	Vec3 v;
	SceneNode* node;
};

class Rotation : public Animation {
public:
	Rotation(SceneNode* node, Qtrn q, float angle) {
		this->node = node;
		this->q = q;
		this->angle = angle;

		float a = angle / 180 * PI;
		duration = a / PI / speed;
	}
	virtual void update(float dt) {
		if (!started) {
			started = true;
			this->start = node->rotation;
			this->end = q * start;
		}
		if (t / duration <= 1.0f) {
			t += dt;
			node->rotation = start.slerp(end, t/duration);
		}
		if (t / duration > 1.0f)
			finish(t / duration - 1.0f);
	}
	virtual Rotation* reverse() {
		return new Rotation(node, q.conjugate(), angle);
	}

	float speed = 0.5f; // 0.5 seconds for a 180 degrees angle
private:
	Qtrn start, end, q;
	SceneNode* node;
	float t = 0;
	float duration;
	float angle;
	bool started = false;
};

class AnimSequence : public Animation, public AnimController {
public:
	AnimSequence(vector<Animation*> animations) {
		this->animations = animations;
		for (auto a : animations)
			a->setController(this);
	}
	~AnimSequence() {
		for (auto a : animations)
			delete a;
	}
	void update(float dt) {
		if (cur < animations.size())
			animations[cur]->update(dt);
	}
	virtual AnimSequence* reverse() {
		vector<Animation*> reversed;
		int size = (int) animations.size();
		reversed.resize(size);
		for (int i = 0; i < size; i++)
			reversed[i] = animations[size - i - 1]->reverse();

		return new AnimSequence(reversed);
	}
	void onFinish(Animation* anim, float dt) {
		if (cur < animations.size() && anim == animations[cur]) {
			cur++;
			if (cur < animations.size())
				animations[cur]->update(dt);
			else
				finish(dt);
		}
	}

private:
	vector<Animation*> animations;
	int cur = 0;
};

