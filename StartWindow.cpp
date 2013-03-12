#include "StartWindow.h"
 
StartWindow::StartWindow() {			
		buttons[0] = Button();
		buttons[0].setName("Join Server").setFunction(&StartWindow::join);
		buttons[1] = Button();
		buttons[1].setName("Exit").setFunction(&StartWindow::exit);
}

void StartWindow::display() {
	glClear(GL_COLOR_BUFFER_BIT);		
	Tools * tool = Tools::getInstance();
		
	for (int i = 0; i < 2; i++) {
		float x = BTN_STARTX;
		float y = BTN_STARTY + (i * (BTN_INSET + BTN_HEIGHT));
			glColor3f(0.7f,0.7f,0.7f);
		glBegin(GL_POLYGON);
		glVertex2f(x , y);
		glVertex2f(x + BTN_WIDTH, y);	
		glVertex2f(x + BTN_WIDTH, y + BTN_HEIGHT);
		glVertex2f(BTN_STARTX , y + BTN_HEIGHT);
		glEnd();
		
		glBegin(GL_POLYGON);	
		glColor3f(0.4f,0.4f,0.4f);
		glVertex2f(x + 2 , 2 + y);
		glVertex2f(x + BTN_WIDTH, 2 + y);	
		glVertex2f(x + BTN_WIDTH, y + BTN_HEIGHT);
		glVertex2f(x + 2, y + BTN_HEIGHT);
		glEnd();
			buttons[i].setMinPos(x,y).setMaxPos(x + BTN_WIDTH, y + BTN_HEIGHT);
			tool->displayText(x + BTN_WIDTH / 2,  y + BTN_HEIGHT / 2, 
			50,0,0,buttons[i].name);
	}
		glutSwapBuffers();
}

void StartWindow::keyboard(unsigned char c, int x, int y) {
}

void StartWindow::mouse(int btn, int state, int x, int y) {
if (state == GLUT_UP) {
		for (Button butt : buttons) {				
		if (x <= butt.maxX && x >= butt.minX && y <= butt.maxY && y >= butt.minY) {
			butt.func();
			glutPostRedisplay();
		}
		}
		}
}
	
void StartWindow::join() {
	std::cout << "join";
	STATE = 1;
	
	changeState();
}
void StartWindow::exit() {
	std::cout << "Exit";
	STATE = 0;
	changeState();
}
