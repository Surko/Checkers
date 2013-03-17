#include "glut.h"
#include "Tools.cpp"
#include "GameWindow.h"
#include "StartWindow.h"
#include "ConnectWindow.h"
#include <iostream>
#include <string>

// Omedzenie vykonu procesoru (nebezi naplno)
static const int FPS_PROHIBITOR = 10;

// Kontajner s Windowom. Kedze je to uplne prvy rodic tak sa v nom mozu nachadzat vsetky ostatne Windowi a volat metody.
Window * win;

// Zaciatocny stav
int Window::STATE = STARTWINDOW;
// premenna urcujuca ci sme napojeny na server
bool Window::connected = false;
// Instancia Toolu. Singleton => len jedna instancia.
Tools * Tools::instance = new Tools();
// Udaje o adrese na ktoru sa napajame
SOCKADDR_IN Window::addr;
// Socket s ktorym sa napajame podla adresy na server.
SOCKET Window::sConnection;
// Vektor sprav do ktorych ukladame prichadzajuce spravy zo serveru.
vector<string> Window::msgQueue;
// Id pripojenia
int Window::connID = 0;

/*
Metoda meni stav okien podla premennej STATE ulozenej vo Window.h.
Podla dvoch parametrov rozhoduje za aku stranu hrac hra a ci to je singleplayer
alebo multiplayer. Po kazdej zmene alokuje novy Window a ten stary odstranuje.
V konzoli sa vzdy ukazuje co za Window akurat bezi.
*/
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

/*
Metoda receiveMsg() ziskava spravy zo socketu zobrazuje co to je za spravu
a vklada do fronty zprav ktoremu su spracovavane Windowmi.
Pri odpojeni serveru spravne ukaze spravu ako v konsoli tak aj v hre.
Metoda bezi na druhom Threade.
*/
DWORD Window::receiveMsg() {

	Buffer sbuffer;
	char buffer[sizeof(sbuffer)] = {0};
	int ret;

	while (STATE > 1) {
		cout << "	Waiting for msg" << endl;
		if ((ret = recv(sConnection, buffer, sizeof(sbuffer), NULL)) != SOCKET_ERROR) {						
			cout << "	Message arrived to " << win -> toString() << endl;
			memcpy(&sbuffer, buffer, 256);
			cout << "	[" << sbuffer.ID << "] -> " << sbuffer.Message << endl;			
			msgQueue.push_back(sbuffer.Message);
		} else {
			cout << "Socket is closed" << endl;			
			Window::connected = false;
			STATE = 1;
			glutPostRedisplay();
		}
	}
	return 0;

}

// Metoda do OPENGL funkcie glutdisplayfunc
void display() {	
	win->display();
}

// Metoda do OPENGL funkcie glutkeyboardfunc
void keyboard(unsigned char c, int x, int y) {
	win->keyboard(c, x, y);
}

// Metoda do OPENGL funkcie glutmousefunc
void mouse(int btn, int state, int x, int y) {
	win->mouse(btn, state, x, y);
}

// Metoda do OPENGL funkcie glutidlefunc. Doplnena o prohibitor (na odlahcenie procesoru)
void update() {
	win->update();
	Sleep(FPS_PROHIBITOR);
}

/*
Hlavna metoda ktora na zaciatku nastavi podla stavu co za Window sa zobrazi.
Potom inicialuzejeme OPENGL a hlavne okno s funkciami na spracovavanie mysi, klavesnice,
zobrazovanie na displej, atd.... Nakonci prevadzame hlavne cyklus.
*/
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