#include <math.h>
#include "GameWindow.h"


GameWindow::GameWindow() {		

}

	
void GameWindow::display() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	for (int x = 0; x < 8; ++x)
		for (int y = 0; y < 8; ++y) 
		{
		if ((x+y) % 2) {
			glColor3f(0.0f,0.0f,0.0f);
		} else { 
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		glBegin(GL_QUADS);
		glVertex2f(x * WIDTH / 8, y * HEIGHT / 8);
		glVertex2f((x+1) * WIDTH / 8, y * HEIGHT / 8);
		glVertex2f((x+1) * WIDTH / 8, (y+1) * HEIGHT / 8);
		glVertex2f(x * WIDTH / 8, (y+1) * HEIGHT / 8);
		glEnd();
			
		switch (game.cell(x,y)) {
		case Game::EMPTY : break;
		case Game::BLACK : 
			glColor3f(0.3f,0.3f,0.3f);
			glBegin(GL_POLYGON);				
			for (int i = 0; i < 360; i++)	{
				float deginRad = i*DEG2RAD;
				glVertex2f(WIDTH / 16 + radius * cos(deginRad) + x * WIDTH / 8,HEIGHT / 16 + radius * sin(deginRad) + y * HEIGHT / 8);
			}
			glEnd();
			break;			
		case Game::WHITE :
			std::pair<int , int> selected = game.selectedCell();
			if (selected.first == x && selected.second == y) {
				glColor3f(1.0,0.0f,0.0f);
			} else {
				glColor3f(0.7f,0.7f,0.7f);
			}

			glBegin(GL_POLYGON);				
			for (int i = 0; i < 360; i++)	{
				float deginRad = i*DEG2RAD;
				glVertex2f(WIDTH / 16 + radius * cos(deginRad) + x * WIDTH / 8,HEIGHT / 16 + radius * sin(deginRad) + y * HEIGHT / 8);
			}
			glEnd();
			break;
		}	
	}

	glutSwapBuffers();
}

void GameWindow::keyboard(unsigned char c, int x, int y) {

}

void GameWindow::mouse(int btn, int state, int x, int y) {
	std::cout << state;
	if (state == GLUT_UP) {
		int _x = x / (WIDTH / 8);
		int _y = y / (HEIGHT / 8);
		if (game.selectedCell() == std::make_pair(-1, -1)) {
			game.selectCell(_x, _y);
		}
		else {
			game.move(_x, _y);
		}			
		glutPostRedisplay();
	}
}

Game * GameWindow::getGame() {
	return &game;
}


