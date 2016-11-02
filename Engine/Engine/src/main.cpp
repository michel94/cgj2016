
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <sstream>
#include <math.h>

#include "vec.hpp"
#include "mat.hpp"
#include "shaders.hpp"
#include "object.hpp"
#include "prettymodels.hpp"

#include "tests.hpp"

using namespace std;

const char CAPTION[] = "Hello Blank World";
const float PI = 3.14159f;

int window;
int windowWidth = 600, windowHeight = 600;
int frameCount;
Shader* shader = NULL;
bool shadersLoaded = false;
bool perspective = false;
Vec2 mouseAngles(3.14159, 0.0);
Vec2 mouseDisp;
bool controls[] = { false, false, false, false, false, false};
Vec3 eye = Vec3(0.5, 0.5, 1);

double lastTick;
vector<Object*> objects;

enum Action {
	KEYPRESS,
	KEYRELEASE
};

double now() { // milliseconds
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (double) ((1000LL * now.QuadPart) / s_frequency.QuadPart);
	}
	else {
		return (double)GetTickCount();
	}
}

/////////////////////////////////////////////////////////////////////// ERRORS


/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void loadObjects() {
	
	Vec4 t = Vec4(0.0f, 0.0f, 0.01f, 0.0f);
	float sq2 = sqrt(2.0f);
	Object* square = new Object(shader, Models<PrettyModel>::getSquarePrism(Vec4(1.0f, 0.0f, 0.0f, 1.0f)));
	square->setModelMatrix(
		Mat4::translate(Vec3(0.5f, 0.5f, 0.0f)) *
		Mat4::scale(Vec3(sq2 / 4, sq2 / 4, 0.85f)) *
		Mat4::rotateAround(Vec3(0, 0, 1), 45)
	);
	objects.push_back(square);
	
	Object* parallelogram = new Object(shader, Models<PrettyModel>::getParallelogramPrism(Vec4(1.0f, 0.0f, 1.0f, 1.0f)) );
	parallelogram->setModelMatrix(
		Mat4::scale(Vec3(0.5f, 0.5f, 0.9f))
	);
	objects.push_back(parallelogram);	

	Object* triangle;

	triangle = new Object(shader, Models<PrettyModel>::getTriangularPrism(Vec4(1.0f, 1.0f, 0.0f, 1.0f)));
	triangle->setModelMatrix(
		Mat4::translate(Vec3(0.5f, 0.5f, 0.0f)) *
		Mat4::scale(Vec3(sq2 / 2, sq2 / 2, 0.4f)) *
		Mat4::rotateAround(Vec3(0, 0, -1), 135)
	);
	objects.push_back(triangle);

	triangle = new Object(shader, Models<PrettyModel>::getTriangularPrism(Vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	triangle->setModelMatrix(
		Mat4::translate(Vec3(0.5f, 0.5f, 0.0f)) * 
		Mat4::scale(Vec3(sq2 / 2, sq2 / 2, 0.5f)) * 
		Mat4::rotateAround(Vec3(0, 0, -1), 45)
	);
	objects.push_back(triangle);

	triangle = new Object(shader, Models<PrettyModel>::getTriangularPrism(Vec4(1.0f, 0.5f, 0.0f, 1.0f)));
	triangle->setModelMatrix(
		Mat4::translate(Vec3(1.0f, 0.0f, 0.0f)) *
		Mat4::scale(Vec3(0.5f, 0.5f, 0.7f)) *
		Mat4::rotateAround(Vec3(0, 0, -1), 90)
	);
	objects.push_back(triangle);
	
	triangle = new Object(shader, Models<PrettyModel>::getTriangularPrism(Vec4(0.0f, 0.0f, 1.0f, 1.0f)));
	triangle->setModelMatrix(
		Mat4::translate(Vec3(0.75f, 0.75f, 0.0f)) *
		Mat4::scale(Vec3(sq2 / 4, sq2 / 4, 0.2f)) *
		Mat4::rotateAround(Vec3(0, 0, -1), -45)
	);
	objects.push_back(triangle);

	
	triangle = new Object(shader, Models<PrettyModel>::getTriangularPrism(Vec4(0.0f, 1.0f, 1.0f, 1.0f)));
	triangle->setModelMatrix(
		Mat4::translate(Vec3(0.5f, 0.5f, 0.0f)) *
		Mat4::scale(Vec3(sq2 / 4, sq2 / 4, 0.8f)) *
		Mat4::rotateAround(Vec3(0, 0, -1), -135)
	);
	objects.push_back(triangle);

}

void destroyObjects(){
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE


void drawScene(float dt) {
	mouseAngles.x += mouseDisp.x;
	mouseAngles.y += mouseDisp.y;
	mouseDisp = Vec2(0, 0);
	Vec3 mouseDir = Vec3(cos(mouseAngles.y)*sin(mouseAngles.x), sin(mouseAngles.y), cos(mouseAngles.y)*cos(mouseAngles.x));
	Vec3 right = Vec3(sin(mouseAngles.x - PI / 2.0f), 0, cos(mouseAngles.x - PI / 2.0f));
	Vec3 up = right.cross(mouseDir);
	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	if (controls[0]) // back
		eye -= 3.0f * dt * mouseDir;
	if (controls[1]) // forward
		eye += 3.0f * dt * mouseDir;
	if (controls[2]) // left
		eye -= 3.0f * dt * right;
	if (controls[3]) // right
		eye += 3.0f * dt * right;
	if (controls[4]) // top
		eye -= 3.0f * dt * up;
	if (controls[5]) // bottom
		eye += 3.0f * dt * up;

	Mat4 VP;
	if (!perspective)
		VP *= Mat4::ortho(-1, 1, -1, 1, 0, 5);
	else {
		VP *= Mat4::perspective(90, windowWidth / (float)windowHeight, 2.0f, 10.0f);
	}
	
	VP *= Mat4::lookAt(eye, eye+mouseDir, up);
	
	for (int i = 0; i < objects.size(); i++)
		objects[i]->draw(VP);
	
	if(shadersLoaded)
		checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup(){
	destroyShader(shader);
	destroyObjects();
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
		perspective = !perspective;
	else if (key == 's')
		controls[4] = action == KEYPRESS;
	else if (key == 'w')
		controls[5] = action == KEYPRESS;
	else if (key == 'a')
		controls[2] = action == KEYPRESS;
	else if (key == 'd')
		controls[3] = action == KEYPRESS;

}
void keyDownFunc(unsigned char key, int x, int y) {
	onKey(key, x, y, KEYPRESS);
}
void keyUpFunc(unsigned char key, int x,  int y) {
	onKey(key, x, y, KEYRELEASE);
}
void onSpecialKey(int key, int x, int y, Action action) {
	if (key == GLUT_KEY_DOWN)
		controls[0] = action == KEYPRESS;
	else if (key == GLUT_KEY_UP)
		controls[1] = action == KEYPRESS;
}
void specialKeyDownFunc(int key, int x, int y) {
	onSpecialKey(key, x, y, KEYPRESS);
}
void specialKeyUpFunc(int key, int x, int y) {
	onSpecialKey(key, x, y, KEYRELEASE);
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

	glutSpecialFunc(specialKeyDownFunc);
	glutSpecialUpFunc(specialKeyUpFunc);
	glutKeyboardFunc(keyDownFunc);
	glutKeyboardUpFunc(keyUpFunc);
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
	drawScene( (float)(curTick - lastTick) / 1000 );
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
	std::ostringstream oss;
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
	shader = loadShader("res/shaders/colored");
	shadersLoaded = shader->loaded;

	glutWarpPointer(windowWidth / 2, windowHeight / 2);
	lastTick = now();
	loadObjects();
	
	setupCallbacks();
}

int main(int argc, char* argv[]) {
	Tests::runTests();
	init(argc, argv);
	glutMainLoop();

	system("pause");
	exit(EXIT_SUCCESS);
}

Vec4 lb = Vec4(0.0f, 0.0f, 0.0f, 1.0f),
lt = Vec4(0.0f, 1.0f, 0.0f, 1.0f),
rb = Vec4(1.0f, 0.0f, 0.0f, 1.0f),
rt = Vec4(1.0f, 1.0f, 0.0f, 1.0f);
