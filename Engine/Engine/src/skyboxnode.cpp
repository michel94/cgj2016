#include "skyboxnode.hpp"

SkyBoxNode::SkyBoxNode(Model * model, SceneNode * parent) : SceneNode(model, parent) {
	shader = ShaderManager::instance().getShader("skybox");
	vector<string> Faces;
	//day images
	Faces.push_back("posx.png");
	Faces.push_back("negx.png");
	Faces.push_back("posy.png");
	Faces.push_back("negy.png");
	Faces.push_back("posz.png");
	Faces.push_back("negz.png");
	
	skyboxDayId = loadCubemap("res/images/", Faces);
	skyboxNightId = loadCubemap("res/images/night_", Faces);
}

GLuint SkyBoxNode::loadCubemap(string folder, vector<string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++){
		cout << folder + faces[i] << endl;
		image = SOIL_load_image((folder + faces[i]).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}

void SkyBoxNode::render(Mat4 tr) {
	if (model && shader) {
		glDepthMask(GL_FALSE);
		Shader& s = *shader;
		s.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxDayId);
		glUniform1i(s["cube_texture"], 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxNightId);
		glUniform1i(s["cube_texture2"], 1);
		glUniform1f(s["blendfactor"], blendfactor);
		glUniformMatrix4fv(s["Matrix"], 1, GL_TRUE, tr.data);
		model->draw();
		s.unbind();
		glDepthMask(GL_TRUE);
	}
}

void SkyBoxNode::update(float dt) {
	SceneNode::rotation *= Qtrn::fromAngleAxis(dt * ROTATE_SPEED, Vec3(0, 1, 0));
	time += dt;
	if(blendfactor >= 1.0f){
		daynight = -1.0f;
	}
	else if(blendfactor <= 0.0f){
		daynight = 1.0f;
		
	}
	blendfactor += 0.05f * daynight*dt;
}

void SkyBoxNode::bindTextures(Shader& s) {
	
}
	