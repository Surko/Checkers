
#include "glut.h"
#include "GameWindow.h"
#include "StartWindow.h"
#include "ConnectWindow.h"
#include <iostream>
#include <string>

Window * win;

int Window::STATE = CONNECTWINDOW;
Tools * Tools::instance = new Tools();

void Window::changeState() {
	delete(win);
		switch (STATE) {
			case 0 :
				exit(0);
			case 1 :			
				win =  new StartWindow();
			break;
			case 2 : 
				win = new GameWindow();
			break;
			case 3 :
				win = new ConnectWindow();
			break;
		}
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

int main(int argc, char ** argv) {	
	switch (Window::STATE) {
	case 0 :
		exit(0);
	case 1 :			
		win =  new StartWindow();
		break;
	case 2 : 
		win = new GameWindow();
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

	glutMainLoop();

}