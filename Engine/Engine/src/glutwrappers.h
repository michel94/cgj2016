#pragma once

enum Action {
	KEYPRESS,
	KEYRELEASE
};

typedef void (*KeyCallback) (unsigned char key, int x, int y, Action action);
typedef void (*SpecialKeyCallback) (int key, int x, int y, Action action);

KeyCallback keyCallback = nullptr;
SpecialKeyCallback specialKeyCallback = nullptr;

void keyDownFunc(unsigned char key, int x, int y) {
	if(keyCallback != nullptr)
		keyCallback(key, x, y, KEYPRESS);
}
void keyUpFunc(unsigned char key, int x, int y) {
	if (keyCallback != nullptr)
		keyCallback(key, x, y, KEYRELEASE);
}

void specialKeyDownFunc(int key, int x, int y) {
	if (specialKeyCallback != nullptr)
		specialKeyCallback(key, x, y, KEYPRESS);
}
void specialKeyUpFunc(int key, int x, int y) {
	if (specialKeyCallback != nullptr)
		specialKeyCallback(key, x, y, KEYRELEASE);
}

void keyFunc(KeyCallback callback) {
	glutKeyboardFunc(keyDownFunc);
	glutKeyboardUpFunc(keyUpFunc);
	keyCallback = callback;
}

void specialKeyFunc(SpecialKeyCallback callback) {
	glutSpecialFunc(specialKeyDownFunc);
	glutSpecialUpFunc(specialKeyUpFunc);
	specialKeyCallback = callback;
}
