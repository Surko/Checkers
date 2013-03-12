#include <math.h>
#include "ConnectWindow.h"
#include <Windows.h>

ConnectWindow::ConnectWindow() {
	wheelCount = 0;
	text[0] = "Connecting";
	text[1] = text[0] + " .";
	text[2] = text[1] + ".";
	text[3] = text[2] + ".";
}

void ConnectWindow::display() {
	Tools * tool = Tools::getInstance();
	
	while (true) {
		glClear(GL_COLOR_BUFFER_BIT);
		tool->displayText(WIDTH / 2, HEIGHT / 2, 50,0,0, text[wheelCount].c_str());
		Sleep(1000);
		wheelCount = (wheelCount + 1) % 4;
		glutSwapBuffers();
	}
}


void ConnectWindow::mouse(int btn, int state, int x, int y) {

}

void ConnectWindow::keyboard(unsigned char c, int x, int y) {

}


