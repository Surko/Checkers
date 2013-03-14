#include <math.h>
#include "GameWindow.h"

typedef std::pair<int, int> Pos;
typedef std::pair<Pos, Pos> Step;

GameWindow::GameWindow(bool side, bool single) {		
	game.setMode(single);
	game.setSide(side);
}

	
void GameWindow::display() {
	glClear(GL_COLOR_BUFFER_BIT);
	std::pair<int , int> selected = game.selectedCell();
		
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
			if (selected.first == x && selected.second == y) {
				glColor3f(1.0,0.0f,0.0f);
			} else {
				glColor3f(0.3f,0.3f,0.3f);
			}			
			glBegin(GL_POLYGON);				
			for (int i = 0; i < 360; i++)	{
				float deginRad = i*DEG2RAD;
				glVertex2f(WIDTH / 16 + radius * cos(deginRad) + x * WIDTH / 8,HEIGHT / 16 + radius * sin(deginRad) + y * HEIGHT / 8);
			}
			glEnd();
			break;			
		case Game::WHITE :			
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
		if (!game.isMyTurn()) {
			Tools * tool = Tools::getInstance();	
			tool->displayText(WIDTH / 2 - 100, HEIGHT / 2, 50, 0, 0, "ENEMY TURN");
		}
	}

	glutSwapBuffers();
}

void GameWindow::update() {

	if (!game.isSinglePlayer()) {
		istringstream iss(receiveMsg());
		string msg;
		iss >> msg;
		if (msg == "MOVE") {

			return;
		}
		if (msg == "DELETE") {

			return;
		}
	}


	if (!game.isMyTurn()) {
		if (game.isSinglePlayer()) {
			game.moveOther();
		}
	}
}

string GameWindow::receiveMsg() {

	Buffer sbuffer;
	char buffer[sizeof(sbuffer)] = {0};
	if (recv(sConnection, buffer, sizeof(sbuffer), NULL)) {
		memcpy(&sbuffer, buffer, sizeof(sbuffer));
			cout << "Player " << sbuffer.ID << " -> " << sbuffer.Message << endl;
			connID = sbuffer.ID;
	}

	return sbuffer.Message;
}


void GameWindow::keyboard(unsigned char c, int x, int y) {

}

void GameWindow::sendMsg(const char *msg) {
	cout << "Sending Message" << endl;
	Buffer sbuffer;
	sbuffer.ID = connID;
	ZeroMemory(sbuffer.Message, 256);
	memcpy(sbuffer.Message, msg, 256);
	char * sendMsg = new char[256];
	memcpy(sendMsg, &sbuffer, sizeof(Buffer));

	if (send(sConnection, sendMsg, sizeof(Buffer), NULL)) {
			cout << "Message -> " << sbuffer.Message << " sent" << endl;
	}
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
			if (game.isMyTurn()) {
				vector<string> msg;
				game.move(_x, _y, msg);
				if (!game.isSinglePlayer()) {
					for (vector<string>::iterator iter = msg.begin(); iter != msg.end(); iter++) {						
						sendMsg(iter->c_str());
					}
				}
			}
		}			
		glutPostRedisplay();
	}
}

Game * GameWindow::getGame() {
	return &game;
}


