#pragma once

#include <vector>
#include "mat.hpp"
#include "models.hpp"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "scene.hpp"

using namespace std;

class Scene;

class SceneNode {
public:
	SceneNode();
	SceneNode(Model * model);
	SceneNode(SceneNode * parent);
	SceneNode(Model * model, SceneNode * parent);
	~SceneNode();
	void setShader(string name);
	void setModelMatrix(Mat4& m);
	
	void addChild(SceneNode* node);
	void addChildren(vector<SceneNode*>& nodes);
	bool removeChild(SceneNode* node);
	void removeChildren();
	void setParent(SceneNode* parent);
	void switchParent(SceneNode* parent);

	void update(float dt);
	virtual void updateChildren(float dt) final;
	void render(Mat4 tr);
	void renderChildren(Mat4 tr);
private:
	//Shader& shader;
	string shaderName;
	Scene* scene;
	Model *model;
	Mat4 mat;

	vector<SceneNode*> children;
	SceneNode* parent;
};

