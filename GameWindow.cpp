#include <math.h>
#include "GameWindow.h"

/*
Konstruktor na GameWindow ktora nastavi podla parametrov side a single samotnu hru.
Side - Strana za ktoru hrame.
Single - Ci hrame singleplayer alebo multiplayer.
*/
GameWindow::GameWindow(bool side, bool single) {		
	game.setMode(single);
	game.setSide(side);
}

/*
Display funkcia pre GameWindow. Sklada sa z vykreslenia plochy 8x8 a vykreslenia
kamenov podla toho ako su ulozene v Game::board
Podla paru selected vykresli oznaceny kamen (nacerveno).
Pri necakanom odpojeni vykresli na prostred Server is down.
Pri konci hry vykresli na prostred konecny stav (vyhra/prehra/remiza)
Pri cudzom tahu vypise na prostred Enemy turn.
*/
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

		if (!connected && !game.isSinglePlayer()) {
			Tools * tool = Tools::getInstance();
			tool -> displayText(WIDTH / 2 - 100, HEIGHT / 2, 50, 0, 0, "SERVER IS DOWN");
		} 

		if (game.isEnd()) {			
			Tools * tool = Tools::getInstance();
			switch (game.getWinStatus()) {
			case -1 : 
				tool -> displayText(WIDTH / 2 - 100, HEIGHT / 2, 50, 0, 0, "REMIZA");
				break;
			case 0 :
				tool -> displayText(WIDTH / 2 - 100, HEIGHT / 2, 50, 0, 0, "YOU LOSE");
				break;
			case 1 :
				tool -> displayText(WIDTH / 2 - 100, HEIGHT / 2, 50, 0, 0, "YOU WON");
				break;
			}
		} else {
			if (!game.isMyTurn()) {
				Tools * tool = Tools::getInstance();	
				tool->displayText(WIDTH / 2 - 100, HEIGHT / 2, 50, 0, 0, "ENEMY TURN");
			}
		}
		
	}

	glutSwapBuffers();
}

/*
Update funkcia pre GameWindow. Sklada sa z kontrolovania poctu kamenov (co nie je dolezite lebo tuto funkciu zabezpeci aj 
ta v Game) a z kontrolovania fronty zprav ktore prisli. Do konzoli vzdy vypise co za sprava prisla.
Command podla ktoreho sa rozhoduje co so spravou spravi. Ked sme v singleplayer mode tak 
iba prepina medzi hracovym tahom a PC tahom.
*/
void GameWindow::update() {	

	if (!game.isSinglePlayer()) {
		if (game.getStones() == 0) {
			game.hasEnded();
			game.setWinStatus(0);
			std::stringstream ss;
			ss << "FINAL " << 0;
			sendMsg(ss.str());				
		}		

		if (!msgQueue.empty()) {
		istringstream iss(msgQueue.front());
		msgQueue.erase(msgQueue.begin());

		cout << "Translating msg" << endl;

		string msg;
		iss >> msg;
		cout << "Command : " << msg << endl;
		if (msg == "MOVE") {
			int sx,sy,ex,ey;
			iss >> sx;
			iss >> sy;
			iss >> ex;
			iss >> ey;
			cout << "	Positions [" << sx << "," << sy << "," << ex << "," << ey << "]" << endl;
			game.move(sx, sy, ex,ey);
			game.setTurn(true);
		}
		if (msg == "DELETE") {
			int x,y;
			iss >> x;
			iss >> y;
			game.del(x,y);
			game.decStones();			
		}	
		if (msg == "FINAL") {
			int x;
			iss >> x;
			
			stringstream ss;
			ss << "FINAL " << game.getStones();
			sendMsg(ss.str());

			game.hasEnded();
			if (x < game.getStones()) {				
				game.setWinStatus(1);
			}
			if (x == game.getStones()) {			
				game.setWinStatus(-1);
			}
			if (x > game.getStones()) {
				game.setWinStatus(0);
			}
			
			STATE = -10;
			closesocket(sConnection);
		}

		if (msg == "RUNAWAY") {
			game.hasEnded();
			game.setWinStatus(1);
			
			STATE = -10;
		}

		glutPostRedisplay();
		}
	}


	if (!game.isMyTurn()) {
		if (game.isSinglePlayer()) {
			game.moveOther();
		}
	}
}

/*
Keyboard funkcia v GameWindow spracovava eventy s klavesnice. V tomto pripade spracovavame iba priznak ESC, ktory naznacuje
ze ukoncujeme sedenie. Informacie sa vypisu na obrazovku a vratime sa do StartWindow.
*/
void GameWindow::keyboard(unsigned char c, int x, int y) {
	

	if (c == VK_ESCAPE) {			
		if (!game.isSinglePlayer()) {
			cout << "DISCONNECTING" << endl;			
		}		

		STATE = 1;
		changeState(false,false);
		glutPostRedisplay();
		closesocket(sConnection);
	}

}

/*
Metoda sendMsg posiela spravu na server. Telo spravi ukaze v konzoli.
Parameter metody je string/sprava ktoru hned odposiela.
*/
void GameWindow::sendMsg(string & msg) {
	cout << "Sending Message" << endl;
	char * sendMsg = new char[256];	
	ZeroMemory(sendMsg, 256);
	memcpy(sendMsg, msg.c_str(), msg.size());	

	if (send(sConnection, sendMsg, 256, NULL)) {
			cout << "Message -> " << sendMsg << " sent" << endl;
	}
}

/*
Mouse funkcia v GameWindow spracovava eventy s mysi. Zaujimame sa iba o stlacenie mysi (prave/lave).
Pri stlaceni na kamen sa kamen oznaci. Pri dalsom stlaceni na plochu posunie kamen.
Pri multiplayer verzii posiela spravy ziskanie z metody move priamo na server ktore spracovava druha strana
*/
void GameWindow::mouse(int btn, int state, int x, int y) {
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
				for (vector<string>::iterator iter = msg.begin(); iter != msg.end(); iter++) {						
					sendMsg(*iter);
				}
			}
		}			
		glutPostRedisplay();
	}
}

/*
Metoda getGame vrati prisluchajucu hru.
*/
Game * GameWindow::getGame() {
	return &game;
}

/*
Metoda vrati string s menom Windovu.
*/
string GameWindow::toString() {
	return string("Game Window");
}
