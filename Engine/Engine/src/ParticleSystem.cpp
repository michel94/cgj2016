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
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STREAM_DRAW);
		glEnableVertexAttribArray(VERTICES);
		glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

		glGenBuffers(1, &vbo_colors_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_colors_id);
		glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(Color), &Colors[0], GL_STREAM_DRAW);
		glEnableVertexAttribArray(COLORS);
		glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Color), 0);
		/*
		glGenBuffers(1, &vbo_psize_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_psize_id);
		glBufferData(GL_ARRAY_BUFFER, Psize.size() * sizeof(float), &Psize[0], GL_STREAM_DRAW);
		glEnableVertexAttribArray(PSIZES);
		glVertexAttribPointer(PSIZES, 1, GL_FLOAT, GL_FALSE, sizeof(float), 0);
		*/
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

ParticleSystem::ParticleSystem(int nParticles) : SceneNode() {
	shader = ShaderManager::instance().getShader("particle");
	MAX_PARTICLES = nParticles;
	
	model = new DynamicModel(nParticles);
}

void ParticleSystem::update(float dt) {
	Particle* p;
	cout << "\r" << "Particles: " << particles.size() << endl;
	while (true) {
		p = createParticle(timeSinceLast);
		if (p != NULL) {
			particles.push_back(p);
		}else {
			timeSinceLast += dt;
			break;
		}
	}

	Model* mesh = model;
	DynamicModel* model = (DynamicModel*)mesh;
	
	int curSize = particles.size();
	for (size_t i = 0; i < curSize; i++){
		updateParticle(particles[i], dt);
		particles[i]->age += dt;
		if (particles[i]->age >= particles[i]->life) {
			swap(particles[i], particles[curSize-- - 1]);
			i--;
		}else {
			memcpy(&model->Vertices[i], particles[i]->position.data(), particles[i]->position.size());
			memcpy(&model->Colors[i], particles[i]->color.data(), particles[i]->color.size());
		}
	}
	if(curSize > 0)
		particles._Pop_back_n(particles.size() - curSize);

	Vec3 camPos = getModelMatrix() * getScene()->getCamera()->position;
	
	//sort(particles.begin(), particles.end(), Less(camPos));
	
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
	glUniform1f(s["w"], particleSize.x);
	glUniform1f(s["h"], particleSize.y);
	
	glBindVertexArray(model->vao_id);
	if(particles.size() > 0)
		glDrawArrays(GL_POINTS, 0, particles.size());
	glBindVertexArray(0);

	s.unbind();
}

void ParticleSystem::renderChildren(Mat4 tr) {
	SceneNode::renderChildren(tr);
}
