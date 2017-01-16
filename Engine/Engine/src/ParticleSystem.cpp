#include "ParticleSystem.hpp"

#include <algorithm>
#include "scene.hpp"

double fRand(double fMin, double fMax) {
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}

double gaussianRandom(double mu, double sigma) {
	const double epsilon = std::numeric_limits<double>::min();
	const double two_pi = 2.0 * atan(1.0);

	static double z0, z1;
	static bool generate;
	generate = !generate;

	if (!generate)
		return z1 * sigma + mu;

	double u1, u2;
	do {
		u1 = rand() * (1.0 / RAND_MAX);
		u2 = rand() * (1.0 / RAND_MAX);
	} while (u1 <= epsilon);

	z0 = sqrt(-2.0 * log(u1)) * cos(two_pi * u2);
	z1 = sqrt(-2.0 * log(u1)) * sin(two_pi * u2);
	return z0 * sigma + mu;
}

DynamicModel::DynamicModel(int maxParticles) {
	this->maxParticles = maxParticles;
	createBuffers();
}

DynamicModel::~DynamicModel() {
	glBindVertexArray(vao_id);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glDisableVertexAttribArray(PSIZES);
	glDeleteBuffers(1, &vbo_vertices_id);
	glDeleteBuffers(1, &vbo_colors_id);
	glDeleteBuffers(1, &vbo_psize_id);
	glDeleteVertexArrays(1, &vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void DynamicModel::createBuffers() {
	Vertices.resize(maxParticles);
	Colors.resize(maxParticles);
	Psize.resize(maxParticles * 2);

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
		
		glGenBuffers(1, &vbo_psize_id);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_psize_id);
		glBufferData(GL_ARRAY_BUFFER, Psize.size() * sizeof(float), &Psize[0], GL_STREAM_DRAW);
		glEnableVertexAttribArray(PSIZES);
		glVertexAttribPointer(PSIZES, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);
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
	cout << "\r" << "Particles: " << particles.size();
	while (particles.size() < MAX_PARTICLES) {
		
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
	//cout << "Psize size: " << model->Psize.size() << " " << model->Psize.size() << endl;
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
			model->Psize[2 * i] = particles[i]->size.x;
			model->Psize[2 * i + 1] = particles[i]->size.y;
		}
	}
	if(curSize > 0)
		particles._Pop_back_n(particles.size() - curSize);

	Vec3 camPos = getModelMatrix() * getScene()->getCamera()->position;
	
	if(sortParticles)
		sort(particles.begin(), particles.end(), Less(camPos));
	
	glBindBuffer(GL_ARRAY_BUFFER, model->vbo_vertices_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model->Vertices.size() * sizeof(Vertex), &model->Vertices[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, model->vbo_colors_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model->Colors.size() * sizeof(Color), &model->Colors[0]);

	glBindBuffer(GL_ARRAY_BUFFER, model->vbo_psize_id);
	glBufferSubData(GL_ARRAY_BUFFER, 0, model->Psize.size() * sizeof(float), &model->Psize[0]);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ParticleSystem::render(Mat4 tr) {
	Shader& s = *shader;
	s.bind();
	glUniformMatrix4fv(s["Matrix"], 1, GL_TRUE, tr.data);

	glBindVertexArray(model->vao_id);
	if(particles.size() > 0)
		glDrawArrays(GL_POINTS, 0, particles.size());
	glBindVertexArray(0);

	s.unbind();
}

void ParticleSystem::renderChildren(Mat4 tr) {
	SceneNode::renderChildren(tr);
}
