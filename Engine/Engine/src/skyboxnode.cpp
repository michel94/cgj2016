#include "skyboxnode.hpp"
#include "scene.hpp"

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
	
	skyboxDayId = loadCubemap("res/images/skybox/day/", Faces);
	skyboxNightId = loadCubemap("res/images/skybox/night/", Faces);
	data = new SkyboxData;
	data->dayTex = skyboxDayId;
	data->nightTex = skyboxNightId;
	getScene()->setSkybox(data);
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
		image = SOIL_load_image((folder + faces[i]).c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
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
		glUniform1f(s["rotation"], angle);
		//printf("blenderfactor skyboxnode = %f\n", blendfactor);
		glUniformMatrix4fv(s["Matrix"], 1, GL_TRUE, tr.data);
		model->draw();
		s.unbind();
		glDepthMask(GL_TRUE);
	}
}

void SkyBoxNode::update(float dt) {
	angle += dt / dayDuration * 180;
	SceneNode::rotation = Qtrn::fromAngleAxis(angle, Vec3(0, 1, 0));
	if(blendfactor >= 1.0f){
		daynight = -1.0f;
	}
	else if(blendfactor <= 0.0f){
		daynight = 1.0f;
		
	}
	blendfactor += 1 / dayDuration * daynight * dt;
	data->blendfactor = blendfactor;
	data->rotation = angle / 180*3.14159f;
}

	