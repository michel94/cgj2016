#pragma once

#include <vector>
#include "quaternion.hpp"
#include "model.hpp"
#include "texture.hpp"
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
	Scene* getScene();
	void switchParent(SceneNode* parent);

	void setTexture(Texture* texture);

	virtual void update(float dt);
	virtual void updateChildren(float dt) final;
	virtual void render(Mat4 tr);
	virtual void renderChildren(Mat4 tr);

	Vec3 position, scale = Vec3(1.0f, 1.0f, 1.0f);
	Qtrn rotation;
    Shader* shader = ShaderManager::instance().getDefaultShader();
	Mat4 mat;
	vector<SceneNode*> children;
	SceneNode* parent;
	Scene* scene = NULL;
protected:
	Model* model;

	Texture * texture = nullptr;

};

class ColoredNode : public SceneNode {
public:
	ColoredNode(Model* model, Vec4 color);
	virtual void render(Mat4 tr);

	Vec4 color;
};

