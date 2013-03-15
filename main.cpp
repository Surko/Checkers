#include "glut.h"
#include "Tools.cpp"
#include "GameWindow.h"
#include "StartWindow.h"
#include "ConnectWindow.h"
#include <iostream>
#include <string>

// Omedzenie vykonu procesoru (nebezi naplno)
static const int FPS_PROHIBITOR = 10;
Window * win;

int Window::STATE = STARTWINDOW;
bool Window::connected = false;
Tools * Tools::instance = new Tools();
SOCKADDR_IN Window::addr;
SOCKET Window::sConnection;
vector<string> Window::msgQueue;
int Window::connID = 0;


void Window::changeState(bool side, bool single) {
	delete(win);
		switch (STATE) {
			case 0 :
				exit(0);
			case 1 :			
				win =  new StartWindow();
			break;
			case 2 : 
				win = new GameWindow(side, single);				
			break;
			case 3 :
				win = new ConnectWindow();
			break;
		}
		cout << win->toString() << endl;
}

DWORD Window::receiveMsg() {

	Buffer sbuffer;
	char buffer[sizeof(sbuffer)] = {0};

	while (STATE > 0) {
		cout << "	" <<  win ->toString() << " waiting for msg" << endl;
		if (recv(sConnection, buffer, sizeof(sbuffer), NULL)) {
			cout << "	Message arrived" << endl;
			memcpy(&sbuffer, buffer, 256);
			cout << "	[" << sbuffer.ID << "] -> " << sbuffer.Message << endl;			
			msgQueue.push_back(sbuffer.Message);
		} 
	}
	return 0;


}


void display() {	
	win->display();
}

void keyboard(unsigned char c, int x, int y) {
	win->keyboard(c, x, y);
}

void mouse(int btn, int state, int x, int y) {
	win->mouse(btn, state, x, y);
}

void update() {
	win->update();
	Sleep(FPS_PROHIBITOR);
}

int main(int argc, char ** argv) {	
	switch (Window::STATE) {
	case 0 :
		exit(0);
	case 1 :			
		win =  new StartWindow();
		break;
	case 2 : 
		win = new GameWindow(0, true);
		break;
	case 3 :
		win = new ConnectWindow();
		break;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH,HEIGHT);
	glutInitWindowPosition(0,95);
	glutCreateWindow("Checkers");
	glClearColor(0, 0, 0, 1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WIDTH, HEIGHT, 0, -1.0, 1.0);	
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);	
	glutIdleFunc(update);

	glutMainLoop();

}