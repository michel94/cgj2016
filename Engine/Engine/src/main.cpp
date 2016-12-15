
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "SOIL.h"
#include <iostream>
#include <string>
#include <sstream>
#include <math.h>

#include "scene.hpp"
#include "vec.hpp"
#include "quaternion.hpp"
#include "shaders.hpp"
#include "modelmanager.hpp"
#include "animation.hpp"
#include "glutwrappers.h"
#include "materialnode.h"

#include "tests.hpp"

using namespace std;

const char CAPTION[] = "Hello Blank World";

int window;
int windowWidth = 640, windowHeight = 480;
int frameCount;
Vec2 mouseDisp;
bool controls[] = { false, false, false, false, false, false };
Scene* scene;
SphericalCamera* camera;
SceneNode* ground;
SceneNode* cube;

double lastTick;
vector<SceneNode*> objects;

double now() { // milliseconds
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (double) ((1000LL * now.QuadPart) / s_frequency.QuadPart);
	}else {
		return (double)GetTickCount();
	}
}

/////////////////////////////////////////////////////////////////////// SCENE SETUP


void loadScene() {
	Mesh* m = ModelManager::instance().getObj("cube");
	scene = new Scene();
	camera = new SphericalCamera(windowWidth, windowHeight);
	scene->attachCamera(camera);
	
	SceneNode* root = scene->root();
	//SceneNode* cube = new MaterialNode(m, root, "stone");
	cube = new MaterialNode(m, root, "stone");
	//SceneNode* sun = new SceneNode(m, root);
	//sun->setTexture(TextureManager::instance().getTexture("brick.png"));
	//sun->position = Vec3(2, 4, 0);
	//root->addChild(sun);
	//Texture* texture = TextureManager::instance().getTexture("sample.png");
/*
	SceneNode* cube = new SceneNode(m, root);
	Texture* texture = TextureManager::instance().get
	Texture("sample_normal.png");
	cube->setTexture(texture);
*/
	
	root->addChild(cube);
	

}

void destroyScene(){
	delete scene;
	checkOpenGLError("ERROR: Could not destroy scene.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void update(float dt) {
	
	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	
	if (controls[0]) { // back
		camera->position -= camera->rotation.toMat4() * Vec3(0.0f, 0.0f, 3.0f) * dt;
	}
	if (controls[1]) { // forward
		camera->position += camera->rotation.toMat4() * Vec3(0.0f, 0.0f, 3.0f) * dt;
	}
	if (controls[2]) { // right
		camera->position += camera->rotation.toMat4() * Vec3(3.0f, 0, 0) * dt;
	}
	if (controls[3]) { // left
		camera->position -= camera->rotation.toMat4() * Vec3(3.0f, 0, 0) * dt;
	}
	if (controls[4]) { // up
		camera->position -= camera->rotation.toMat4() * Vec3(0, 3.0f, 0) * dt;
	}
	if (controls[5]) { // down
		camera->position += camera->rotation.toMat4() * Vec3(0, 3.0f, 0) * dt;
	}
	
	AnimManager::instance().update(dt);

	camera->rotation *= Qtrn::fromAngleAxis(mouseDisp.x * 100, Vec3(0, 1, 0)) * Qtrn::fromAngleAxis(-mouseDisp.y * 100, Vec3(1, 0, 0));
	mouseDisp = Vec2(0, 0);
	scene->update(dt);
	
	if(ShaderManager::instance().shadersLoaded())
		checkOpenGLError("ERROR: Could not draw scene.");
}

void render() {
	scene->render();
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup(){
	destroyScene();
}

void checkOpenGLInfo() {
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cerr << "OpenGL Renderer: " << renderer << " (" << vendor << ")" << endl;
	cerr << "OpenGL Version: " << version << endl;
	cerr << "OpenGL Renderer: " << glslVersion << endl;
}

void setupOpenGL() {
	checkOpenGLInfo();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glDisable(GL_BLEND);
}


void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit();
	if (result != GLEW_OK) {
		cerr << "Error glewInit: " << glewGetString(result) << endl;
		exit(EXIT_FAILURE);
	}
	GLenum errCode = glGetError();
}

void onMouseMoved(int x, int y) {
	Vec2 center(windowWidth / 2.0f, windowHeight / 2.0f);
	float dx = (center.x - x) / windowWidth , dy = (center.y - y) / windowHeight;
	mouseDisp.x += dx;
	mouseDisp.y += dy;
}

void onKey(unsigned char key, int x, int y, Action action) {
	if (key == 27)
		glutDestroyWindow(window);
	else if (key == 'p' && action == KEYRELEASE)
		camera->perspective = !camera->perspective;
	else if (key == 's')
		controls[4] = action == KEYPRESS;
	else if (key == 'w')
		controls[5] = action == KEYPRESS;
	else if (key == 'a')
		controls[2] = action == KEYPRESS;
	else if (key == 'd')
		controls[3] = action == KEYPRESS;

}

void onSpecialKey(int key, int x, int y, Action action) {
	if (key == GLUT_KEY_DOWN)
		controls[0] = action == KEYPRESS;
	else if (key == GLUT_KEY_UP)
		controls[1] = action == KEYPRESS;
}

void setupGLUT(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	window = glutCreateWindow(CAPTION);
	if (window < 1) {
		cerr << "Error: Could not create rendering window." << endl;
		exit(EXIT_FAILURE);
	}
	keyFunc(onKey);
	specialKeyFunc(onSpecialKey);
	glutPassiveMotionFunc(onMouseMoved);
	glutSetCursor(GLUT_CURSOR_NONE);
}

void display() {
	++frameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);

	double curTick = now();
	update( (float)(curTick - lastTick) / 1000 );
	render();
	lastTick = curTick;
	glutSwapBuffers();
}

void idle() {
	glutPostRedisplay();
}

void reshape(int w, int h) {
	windowWidth = w;
	windowHeight = h;
	glViewport(0, 0, w, h);
}

void timer(int value) {
	ostringstream oss;
	oss << CAPTION << ": " << frameCount << " FPS @ (" << windowWidth << " x " << windowHeight << ") " << endl;
	string s = oss.str();
	glutSetWindow(window);
	glutSetWindowTitle(s.c_str());
	frameCount = 0;
	glutTimerFunc(1000, timer, 0);
}

void setupCallbacks() {
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);	
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0, timer, 0);
}

void init(int argc, char* argv[]) {
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();

	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	lastTick = now();
	loadScene();
	//camera->rotation = Qtrn::fromAngleAxis(0, Vec3(0, 1, 0));
	camera->rotation = Qtrn::fromAngleAxis(135, Vec3(1, 0, 0));
	camera->position = Vec3(0, 2, 2);
	
	setupCallbacks();
}

int main(int argc, char* argv[]) {
	Tests::runTests();

	init(argc, argv);
	glutMainLoop();

	system("pause");
	exit(EXIT_SUCCESS);
}
