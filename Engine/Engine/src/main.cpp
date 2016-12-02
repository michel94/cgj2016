
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

SceneNode* triangle1, *triangle2, *triangle3, *triangle4, *triangle5;
SceneNode* square, *parallelogram;

void loadTransformations() {
	float sq2 = sqrt(2.0f);

	square->position = Vec3(0.5f, 0.5f, 0.0f);
	square->scale = Vec3(sq2 / 8, sq2 / 8, sq2 / 8);
	square->rotation = Qtrn::fromAngleAxis(45, Vec3(0, 0, 1));

	parallelogram->scale = Vec3(0.25f, 0.25f, 0.15f);

	triangle1->position = Vec3(0.5f, 0.5f, 0.0f);
	triangle1->scale = Vec3(sq2 / 2, sq2 / 2, 0.3f);
	triangle1->rotation = Qtrn::fromAngleAxis(135.0f, Vec3(0, 0, -1));

	triangle2->position = Vec3(0.5f, 0.5f, 0.0f);
	triangle2->scale = Vec3(sq2 / 2, sq2 / 2, 0.25f);
	triangle2->rotation = Qtrn::fromAngleAxis(45, Vec3(0, 0, -1));

	triangle3->position = Vec3(1.0f, 0.0f, 0.0f);
	triangle3->scale = Vec3(0.5f, 0.5f, 0.3f);
	triangle3->rotation = Qtrn::fromAngleAxis(90, Vec3(0, 0, -1));

	triangle4->position = Vec3(0.75f, 0.75f, 0.0f);
	triangle4->scale = Vec3(sq2 / 4, sq2 / 4, 0.2f);
	triangle4->rotation = Qtrn::fromAngleAxis(-45, Vec3(0, 0, -1));
	
	triangle5->position = Vec3(0.5f, 0.5f, 0.0f);
	triangle5->scale = Vec3(sq2 / 4, sq2 / 4, 0.3f);
	triangle5->rotation = Qtrn::fromAngleAxis(-135, Vec3(0, 0, -1));
}

void startAnimations(bool reverse) {
	vector<Animation*> animations = {
		new AnimSequence({
			new Translation(square, Vec3(0, 0, 0.5f)),
			new Translation(square, Vec3(-0.15f, 0.375f, 0)),
			new Translation(square, Vec3(0, 0, -0.5f)),
		}),
		new AnimSequence({
			new Translation(parallelogram, Vec3(0, 0, 0.3f)),
			new Rotation(parallelogram, Qtrn::fromAngleAxis(180.0, Vec3(1, 0, 0)), 180.0f),
			new Rotation(parallelogram, Qtrn::fromAngleAxis(90.0, Vec3(0, 0, 1)), 90.0f),
			new Translation(parallelogram, Vec3(0.35f, 0.9f, 0)),
		}),
		new AnimSequence({
			new Translation(triangle1, Vec3(0, 0, 0.8f)),
			new Rotation(triangle1, Qtrn::fromAngleAxis(-45.0f, Vec3(0, 0, -1)), 45.0f),
			new Translation(triangle1, Vec3(0.0f, -0.5f, 0)),
			new Translation(triangle1, Vec3(0, 0, -0.8f)),
		}),
		new AnimSequence({
			new Translation(triangle2, Vec3(0, 0, 1.2f)),
			new Translation(triangle2, Vec3(-0.5f, -0.3f, 0)),
			new Translation(triangle2, Vec3(0, 0, -1.2f)),
		}),
		new AnimSequence({
			new Translation(triangle3, Vec3(0, 0, 0.8f)),
			new Rotation(triangle3, Qtrn::fromAngleAxis(45.0f, Vec3(0, 0, -1)), 45.0f),
			new Translation(triangle3, Vec3(-0.14f, 0.35f, 0.0f)),
			new Translation(triangle3, Vec3(0.0f, 0.0f, -0.8f)),
		}),
		new AnimSequence({
			new Translation(triangle4, Vec3(-0.15f, -0.125f, 0.0f)),
		}),
		new AnimSequence({
			new Translation(triangle5, Vec3(0, 0, 1.4f)),
			new Rotation(triangle5, Qtrn::fromAngleAxis(135.0f, Vec3(0, 0, -1)), 135.0f),
			new Translation(triangle5, Vec3(0.1f, 0.8f, 0)),
			new Translation(triangle5, Vec3(0, 0, -1.4f)),
		}),
	};
	if (reverse) {
		for (int i = 0; i < animations.size(); i++)
			animations[i] = animations[i]->reverse();
	}

	AnimManager::instance().start(animations);
}

void loadScene() {
	Mesh* m = ModelManager::instance().getObj("cube");
	 scene = new Scene();
	camera = new SphericalCamera(windowWidth, windowHeight);
	scene->attachCamera(camera);
	SceneNode* root = scene->root();
	SceneNode* cube = new SceneNode(m, root);
	Shader* shader = ShaderManager::instance().getShader("colored");
	cube->setShader(shader);
	Texture* texture = TextureManager::instance().getTexture("sample.png");

	cube->setTexture(texture);
	root->addChild(cube);

	
}

//void loadScene() {
//	Mesh* m = ModelManager::instance().getObj("triangle");
//	float sq2 = sqrt(2.0f);
//	square = new ColoredNode(ModelManager::instance().getObj("cube"), Vec4(1.0f, 0.0f, 0.0f, 1.0f));
//	parallelogram = new ColoredNode(ModelManager::instance().getObj("parallelogram"), Vec4(1.0f, 0.0f, 1.0f, 1.0f));
//	triangle1 = new ColoredNode(m, Vec4(1.0f, 1.0f, 0.0f, 1.0f));
//	triangle2 = new ColoredNode(m, Vec4(0.0f, 1.0f, 0.0f, 1.0f));
//	triangle3 = new ColoredNode(m, Vec4(1.0f, 0.5f, 0.0f, 1.0f));
//	triangle4 = new ColoredNode(m, Vec4(0.0f, 0.0f, 1.0f, 1.0f));
//	triangle5 = new ColoredNode(m, Vec4(0.0f, 1.0f, 1.0f, 1.0f));
//	
//	objects.push_back(square);
//	objects.push_back(parallelogram);
//	objects.push_back(triangle1);
//	objects.push_back(triangle2);
//	objects.push_back(triangle3);
//	objects.push_back(triangle4);
//	objects.push_back(triangle5);
//
//	SceneNode* plane = new ColoredNode(ModelManager::instance().getSquare(), Vec4(0.5, 0.5, 0.5, 1));
//	plane->position = Vec3(-2, -2, 0);
//	plane->scale = Vec3(4.0f, 4.0f, 4.0f);
//	loadTransformations();
//	
//	scene = new Scene();
//	camera = new SphericalCamera(windowWidth, windowHeight);
//	scene->attachCamera(camera);
//	SceneNode* root = scene->root();
//	ground = new SceneNode();
//	root->addChild(ground);
//	ground->addChildren(objects);
//	ground->addChild(plane);
//}

void destroyScene(){
	delete scene;
	checkOpenGLError("ERROR: Could not destroy scene.");
}

/////////////////////////////////////////////////////////////////////// SCENE

void update(float dt) {
	
	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	//Mat4 mMove;
	if (controls[0]) { // back
		camera->dist += 3.0f * dt;
	}
	if (controls[1]) { // forward
		camera->dist -= 3.0f * dt;
	}
	//if (controls[2]) { // left
	//	ground->position += Vec3(3.0f, 0, 0) * dt;
	//}
	//if (controls[3]) { // right
	//	ground->position += Vec3(-3.0f, 0, 0) * dt;
	//}
	//if (controls[4]) { // top
	//	ground->position += Vec3(0, 3.0f, 0) * dt;
	//}
	//if (controls[5]) { // bottom
	//	ground->position += Vec3(0, -3.0f, 0) * dt;
	//}

	AnimManager::instance().update(dt);

	camera->rotation *= Qtrn::fromAngleAxis(mouseDisp.x * 100, Vec3(0, 1, 0)) * Qtrn::fromAngleAxis(mouseDisp.y * 100, Vec3(1, 0, 0));
	camera->matRotation = Mat4::rotateAround(Vec3(0, 1, 0), mouseDisp.x * 100) * Mat4::rotateAround(Vec3(1, 0, 0), mouseDisp.y * 100) * camera->matRotation;
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
	else if (key == 'k' && action == KEYRELEASE)
		startAnimations(false);
	else if (key == 'l' && action == KEYRELEASE)
		startAnimations(true);

}

void onSpecialKey(int key, int x, int y, Action action) {
	if (key == GLUT_KEY_DOWN)
		controls[0] = action == KEYPRESS;
	else if (key == GLUT_KEY_UP)
		controls[1] = action == KEYPRESS;
}

void setupGLUT(int argc, char* argv[]) {
	glutInit(&argc, argv);

	glutInitContextVersion(4, 5);
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
	camera->rotation = Qtrn::fromAngleAxis(0, Vec3(0, 1, 0));
	
	setupCallbacks();
}

int main(int argc, char* argv[]) {
	Tests::runTests();

	//int width, height;
	//unsigned char* image =
	//	SOIL_load_image("sample.png", &width, &height, 0, SOIL_LOAD_RGB);

	//SOIL_free_image_data(image);

	init(argc, argv);
	glutMainLoop();

	system("pause");
	exit(EXIT_SUCCESS);
}
