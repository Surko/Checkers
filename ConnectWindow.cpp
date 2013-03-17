#include <math.h>
#include "ConnectWindow.h"

/*
Konstruktor pre ConnectWindow. Nastavi private premenne na pocitacovne hodnoty
a textove pole na texty oznamujuce napajanie na server kedze to bude toto okno vykonavat.
*/
ConnectWindow::ConnectWindow() {
	side = -1;
	wheelCount = 0;
	connected = 0;
	text[0] = "Connecting";
	text[1] = text[0] + " .";
	text[2] = text[1] + ".";
	text[3] = text[2] + ".";
}

/*
Display funkcia pre ConnectWindow. Sklada sa z vykreslenia textu
ulozenom v poli textov.
*/
void ConnectWindow::display() {
	Tools * tool = Tools::getInstance();	
	glClear(GL_COLOR_BUFFER_BIT);
	tool->displayText(WIDTH / 2, HEIGHT / 2, 50,0,0, text[wheelCount].c_str());
	Sleep(1000);
	wheelCount = (wheelCount + 1) % 4;
	glutSwapBuffers();		
}

/*
Update funkcia pre ConnectWindow. V prvom rade kontroluje ci sme napojeny na server.
Ked nie tak sa snazi o napojenie -> pri uspechu nastavi premennu connected na true a pri neuspechu
poziada o vykreslenie => pridavanie bodiek ... cakanie na server.
Ked sme sa napojili na server (connected == true) tak kontroluje frontu zprav ktore zpracovava podla prikazov relevantnych
pre toto okno. Prikazy relevantne su WELCOME a OPONENT. WELCOME spravov server vita hraca a priradujeme
mu stranu za ktoru bude hrat. Ked neexistuje nepriatel napojeny na servery tak  zmeny vykreslovaci text na sadu textov oznamujuce
cakanie na hraca (na prichod spravy OPONENT). Pri prichode spravy OPONENT vieme ze je napojeny nepriatel na serveri.
Prehodi obrazovku na GameWindow a odtial sa zacne vyuzivat update funkcia pre GameWindow.
Vsetky udalosti/informacie od serveru sa zobrazuju v konzoli.
*/
void ConnectWindow::update() {
	Tools * tool = Tools::getInstance();
	if (!connected) {
		if (tryConnect()) {
		connected = true;			
		} else {
			glutPostRedisplay();
		}
	} else {				
		if (!msgQueue.empty()) {	
			string msg = msgQueue.front();
			msgQueue.erase(msgQueue.begin());
			cout << msg << endl;
			istringstream iss(msg);				
			iss >> msg;
			if (msg == "WELCOME") {
			iss >> msg;
			side = atoi(msg.c_str());
			cout << "Your side: " << (side == true ? "WHITE" : "BLACK") << endl;

			text[0] = "Waiting for oponent";
			text[1] = text[0] + " .";
			text[2] = text[1] + ".";
			text[3] = text[2] + ".";
			}

			if (msg == "OPONENT") {		
				// zmenenie na GameWindow	
				STATE = 2;
				// Je to multiplayer takze parameter v changeState bude false
				if (side != -1)
					changeState(side, false);
			}
		}
		glutPostRedisplay();
	}
}

/*
Metoda tryConnect sa snazi napojit na server. Pri roznych tazkostiach pri napojeni je navratena hodnota 0 ktora znamena
ze server je nejakym sposobom nepristupny. Pri napajani zobrazuje vsetky informacie do konzoli.
Po napojeni vypise informaciu o napojeni a zapne Thread na ziskavanie sprav receiveMsg.
*/
int ConnectWindow::tryConnect() {
		char message[200];
		std::string strmessage;
	
		long answer;
		WSAData wsaData;
		WORD dll_version;
		dll_version = MAKEWORD(2,1);
		answer = WSAStartup(dll_version, &wsaData);
	
		if (answer) {
			return 0;
		}
	
		int adrlen = sizeof(addr);
	
		sConnection = socket(AF_INET, SOCK_STREAM, NULL);
		if (sConnection == INVALID_SOCKET) {
			return 0;
		}
		
		addr.sin_addr.s_addr = inet_addr(hostname);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
	
		std::cout << "Connecting ..." << std::endl;
	
		if (connect(sConnection, (SOCKADDR *)&addr,sizeof(addr))==SOCKET_ERROR) {
			std::cout << "Socket Error" << std::endl;	
			return 0;
		} else {
			std::cout << "CONNECTED" << std::endl;	
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) receiveMsg, NULL, NULL, NULL);
		}
		
		return 1;
}

/*
Prazdna mouse funkcia. Mozne doplnit v buducnosti.
*/
void ConnectWindow::mouse(int btn, int state, int x, int y) {

}

/*
Prazdna keyboard funkcia. Mozne doplnit v buducnosti.
*/
void ConnectWindow::keyboard(unsigned char c, int x, int y) {

}

/*
Metoda toString vrati meno tejto triedy v stringu.
*/
string ConnectWindow::toString() {
	return string("Connect Window");
}