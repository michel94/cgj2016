
#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <sstream>
#include <math.h>

#include "vec.hpp"
#include "mat.hpp"

#include "tests.hpp"

using namespace std;

const char CAPTION[] = "Hello Blank World";

int window;
int windowWidth = 800, windowHeight = 600;
int frameCount;

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

void cleanup() {

}

void display() {
	++frameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
	setupCallbacks();
}

float rand01() {
	return (float)(rand() % 100000) / 100000;
}

Vec3 randVec3() {
	return Vec3(rand01(), rand01(), rand01());
}

void vecTests() {
	/*Vec3::test();
	Vec2::test();

	srand((unsigned int)time(NULL));
	int t = 10;
	while (t--) {
		Vec3 i = randVec3(),
			j = randVec3(),
			k = randVec3();
		Vec3 v1 = i.cross(j.cross(k)),
			v2 = j * i.dot(k) - k * i.dot(j);
		cout << "Run " << 10 - t << ":" << endl;
		cout << i << j << k << endl;
		cout << v1 << v2 << endl;
		assert(v1 == v2);
		//assert(v1 - v2 <= Vec3(0.00001, 0.00001, 0.00001) && v1 - v2 >= Vec3(-0.00001, -0.00001, -0.00001));
		
	}*/

}

int main(int argc, char* argv[]) {
	//init(argc, argv);
	//glutMainLoop();

	Tests::runTests();

	system("pause");
	exit(EXIT_SUCCESS);
}
