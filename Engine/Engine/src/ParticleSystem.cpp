#include "ParticleSystem.hpp"

#include <algorithm>
#include "scene.hpp"

double fRand(double fMin, double fMax) {
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

DynamicModel::DynamicModel(int maxParticles) {
	this->maxVertices = maxParticles * 6;
	createBuffers();
}

DynamicModel::~DynamicModel() {
	glBindVertexArray(vao_id);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDeleteBuffers(1, &vbo_vertices_id);
	glDeleteBuffers(1, &vbo_colors_id);
	glDeleteVertexArrays(1, &vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void DynamicModel::createBuffers() {
	Vertices.resize(maxVertices);
	Colors.resize(maxVertices);

	glGenVertexArrays(1, &vao_id);
	glBindVertexArray(vao_id);
	{
		glGenBuffers(1, &vbo_vertices_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_id);
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glGenBuffers(1, &vbo_colors_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_colors_id);
		glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(Color), &Colors[0], GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(COLORS);
		glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Color), 0);
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

ParticleSystem::ParticleSystem(int nParticles) : SceneNode() {
	shader = ShaderManager::instance().getShader("particle");
	MAX_PARTICLES = nParticles;
	
	model = new DynamicModel(nParticles);
}

SceneNode* ParticleSystem::getParticleNode() {
	Mesh* m = ModelManager::instance().getObj("plane");
	
	ColoredNode* droplet = new ColoredNode(m, Vec4(0, 0, 0, 1));
	droplet->parent = this;

	float randomX = fRand(-2.0f, 2.0f);
	float randomY = fRand(-5.0f, 5.0f);
	float randomZ = fRand(-2.0f, 2.0f);
	droplet->position = Vec3(randomX, randomY, randomZ);
	
	droplet->setShader(shader);

	return droplet;
}

void ParticleSystem::update(float dt) {
	Particle* p;
	//cout << "\r" << "Particles: " << particles.size() << endl;
	while (true) {
		p = createParticle(timeSinceLast);
		if (p != NULL) {
			particles.push_back(p);
		}else {
			timeSinceLast += dt;
			break;
		}
	}
	
	for (size_t i = 0; i < particles.size(); i++){
		updateParticle(particles[i], dt);
		particles[i]->age += dt;
		if (particles[i]->age >= particles[i]->life) {
			particles.erase(particles.begin() + i);
			i--;
		}
	}
	Vec3 camPos = getModelMatrix() * getScene()->getCamera()->position;
	//sort(children.begin(), children.end(), Less(camPos));
	Model* mesh = model;
	DynamicModel* model = (DynamicModel*) mesh;
	float d = 0.01;
	for (int i = 0; i < particles.size(); i++) {
		Vec3 vert[6] = { Vec3(0.0f, 0.0f, 0.0f), Vec3(d, d, 0.0f), Vec3(0.0f, d, 0.0f),
						 Vec3(0.0f, 0.0f, 0.0f), Vec3(d, 0.0f, 0.0f), Vec3(d, d, 0.0f) };
		Vec4 color = particles[i]->color;
		for (int v = 0; v < 6; v++) {
			vert[v] += particles[i]->position;
			memcpy(&model->Vertices[i * 6 + v], vert[v].data(), vert[v].size());
			memcpy(&model->Colors[i * 6 + v], color.data(), color.size());
		}
	}
	glBindBuffer(GL_ARRAY_BUFFER, model->vbo_vertices_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model->Vertices.size() * sizeof(Vertex), &model->Vertices[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, model->vbo_colors_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model->Colors.size() * sizeof(Color), &model->Colors[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void ParticleSystem::render(Mat4 tr) {
	Shader& s = *shader;
	s.bind();
	glUniformMatrix4fv(s["Matrix"], 1, GL_TRUE, tr.data);
	
	glBindVertexArray(model->vao_id);
	if(particles.size() > 0)
		glDrawArrays(GL_TRIANGLES, 0, particles.size() * 6);
	glBindVertexArray(0);

	s.unbind();
}

void ParticleSystem::renderChildren(Mat4 tr) {
	SceneNode::renderChildren(tr);
}

