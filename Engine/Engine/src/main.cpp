
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

#include "tests.hpp"

using namespace std;

const char CAPTION[] = "Hello Blank World";

int window;
int windowWidth = 600, windowHeight = 600;
int frameCount;
Shader* shader = NULL;
Object* triangle = NULL, *square = NULL, *parallelogram = NULL, *limits = NULL;

long long now() { // milliseconds
	static LARGE_INTEGER s_frequency;
	static BOOL s_use_qpc = QueryPerformanceFrequency(&s_frequency);
	if (s_use_qpc) {
		LARGE_INTEGER now;
		QueryPerformanceCounter(&now);
		return (1000LL * now.QuadPart) / s_frequency.QuadPart;
	}
	else {
		return GetTickCount();
	}
}

#define VERTICES 0
#define COLORS 1

GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

/////////////////////////////////////////////////////////////////////// ERRORS


/////////////////////////////////////////////////////////////////////// VAOs & VBOs

void loadObjects() {
	Vec4 lb = Vec4(0.0f, 0.0f, 0.0f, 1.0f),
		 lt = Vec4(0.0f, 1.0f, 0.0f, 1.0f),
		 rb = Vec4(1.0f, 0.0f, 0.0f, 1.0f),
		 rt = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	Vec4 t = Vec4(0.0f, 0.0f, 0.01f, 0.0f);
	triangle = new Object(shader, {lb,rb,lt}, Vec4(0.0f, 1.0f, 0.0f, 0.0f), {0, 1, 2});
	square = new Object(shader, {lb, rb, lt, rt}, Vec4(1.0f, 0.0f, 0.0f, 1.0f), {1, 3, 2, 0, 1, 2});
	limits = new Object(shader, { lb+t, rb+t, lt+t, rt+t }, Vec4(1.0f, 1.0f, 1.0f, 1.0f), { 1, 3, 2, 0, 1, 2 });
	parallelogram = new Object(shader, { lb, rb, Vec4(0.5f, 0.5f, 0.0f, 1.0f), Vec4(1.5f, 0.5f, 0.0f, 1.0f) }, 
		Vec4(1.0f, 0.0f, 1.0f, 1.0f), { 1, 3, 2, 0, 1, 2 });
}

void destroyObjects()
{
	delete triangle;

	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE


void drawScene() {
	Mat4 world = Mat4::translate(Vec3(-1, -1, 0)) * Mat4::scale(Vec3(2, 2, 1));
	
	float sq2 = sqrt(2.0f);
	//limits->draw(world);

	Mat4 tr;
	tr *= Mat4::translate(Vec3(0.5f, 0.5f, 0.0f));
	tr *= Mat4::scale(Vec3(sq2/2, sq2/2, 0.5f));
	tr *= Mat4::rotateAround(Vec3(0, 0, -1), 135);
	triangle->setColor(Vec4(1.0f, 1.0f, 0.0f, 1.0f));
	triangle->draw(world*tr);

	tr = Mat4();
	tr *= Mat4::translate(Vec3(0.5f, 0.5f, 0.0f));
	tr *= Mat4::scale(Vec3(sq2/2, sq2/2, 0.5f));
	tr *= Mat4::rotateAround(Vec3(0, 0, -1), 45);
	triangle->setColor(Vec4(0.0f, 1.0f, 0.0f, 1.0f));
	triangle->draw(world*tr);

	tr = Mat4();
	tr *= Mat4::translate(Vec3(1.0f, 0.0f, 0.0f));
	tr *= Mat4::scale(Vec3(0.5f, 0.5f, 0.5f));
	tr *= Mat4::rotateAround(Vec3(0, 0, -1), 90);
	triangle->setColor(Vec4(1.0f, 0.5f, 0.0f, 1.0f));
	triangle->draw(world*tr);

	tr = Mat4();
	tr *= Mat4::translate(Vec3(0.75f, 0.75f, 0.0f));
	tr *= Mat4::scale(Vec3(sq2/4, sq2/4, 1));
	tr *= Mat4::rotateAround(Vec3(0, 0, -1), -45);
	triangle->setColor(Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	triangle->draw(world*tr);
	
	tr = Mat4();
	tr *= Mat4::translate(Vec3(0.5f, 0.5f, 0.0f));
	tr *= Mat4::scale(Vec3(sq2/4, sq2/4, 1));
	tr *= Mat4::rotateAround(Vec3(0, 0, -1), -135);
	triangle->setColor(Vec4(0.0f, 1.0f, 1.0f, 1.0f));
	triangle->draw(world*tr);
	
	tr = Mat4();
	tr *= Mat4::translate(Vec3(0.5f, 0.5f, 0.0f));
	tr *= Mat4::scale(Vec3(sq2/4, sq2/4, 1.0f));
	tr *= Mat4::rotateAround(Vec3(0, 0, 1), 45);
	square->setColor(Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	square->draw(world*tr);

	tr = Mat4();
	tr *= Mat4::scale(Vec3(0.5f, 0.5f, 1.0f));
	parallelogram->setColor(Vec4(1.0f, 0.0f, 1.0f, 1.0f));
	parallelogram->draw(world*tr);
	
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
}

void display() {
	++frameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
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
	loadObjects();
	
	setupCallbacks();
}

int main(int argc, char* argv[]) {
	Tests::runTests();
	init(argc, argv);
	glutMainLoop();

	exit(EXIT_SUCCESS);
	system("pause");
}
