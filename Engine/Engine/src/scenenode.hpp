#pragma once

#include <vector>
#include "quaternion.hpp"
#include "model.hpp"
#include <GL/glew.h>
#include <GL/freeglut.h>

class Scene;

using namespace std;

class SceneNode {
public:
	SceneNode();
	SceneNode(Model * model);
	SceneNode(SceneNode * parent);
	SceneNode(Model * model, SceneNode * parent);
	~SceneNode();
	void setShader(Shader* s);
	Mat4 getModelMatrix();
	void setModelMatrix(Mat4 m);

	Mat4 calcModelMatrix();
	
	void addChild(SceneNode* node);
	void addChildren(vector<SceneNode*>& nodes);
	bool removeChild(SceneNode* node);
	void removeChildren();
	void setParent(SceneNode* parent);
	void switchParent(SceneNode* parent);

	virtual void update(float dt);
	virtual void updateChildren(float dt) final;
	virtual void render(Mat4 tr);
	virtual void renderChildren(Mat4 tr) final;

	Vec3 position, scale = Vec3(1.0f, 1.0f, 1.0f);
	Qtrn rotation;

	Mat4 mat;
protected:
	Scene* scene;
	Model *model;

	vector<SceneNode*> children;
	SceneNode* parent;
	Shader* shader = ShaderManager::instance().getDefaultShader();
};

class ColoredNode : public SceneNode {
public:
	ColoredNode(Model* model, Vec4 color);
	virtual void render(Mat4 tr);

	Vec4 color;
};

