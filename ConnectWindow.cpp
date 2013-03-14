#include <math.h>
#include "ConnectWindow.h"

ConnectWindow::ConnectWindow() {
	wheelCount = 0;
	connected = 0;
	text[0] = "Connecting";
	text[1] = text[0] + " .";
	text[2] = text[1] + ".";
	text[3] = text[2] + ".";
}

void ConnectWindow::display() {
	Tools * tool = Tools::getInstance();	
	glClear(GL_COLOR_BUFFER_BIT);
	tool->displayText(WIDTH / 2, HEIGHT / 2, 50,0,0, text[wheelCount].c_str());
	Sleep(1000);
	wheelCount = (wheelCount + 1) % 4;
	glutSwapBuffers();		
}

void ConnectWindow::update() {
	Tools * tool = Tools::getInstance();
	if (tryConnect()) {
		connected = true;
		// zmenenie na GameWindow
		STATE = 2;
		// ziskaj spravu ked sme napojeny
		// prekladanie spravy
		string msg = receiveMsg();
		istringstream iss(msg);			
		iss >> msg;
		iss >> msg;
		bool side = atoi(msg.c_str());
		// Je to multiplayer takze parameter v changeState bude false
		changeState(side, false);
		// Zavola aby OpenGL prekreslilo
		glutPostRedisplay();
	} else {
		// Ked sa nepodari tak zavola vykreslenie podla metody display v tomto connectWindow.
		glutPostRedisplay();
	}
}

string ConnectWindow::receiveMsg() {

	Buffer sbuffer;
	char buffer[sizeof(sbuffer)] = {0};
	if (recv(sConnection, buffer, sizeof(sbuffer), NULL)) {
		memcpy(&sbuffer, buffer, sizeof(sbuffer));
			cout << "Player " << sbuffer.ID << " -> " << sbuffer.Message << endl;
			connID = sbuffer.ID;
	}

	return sbuffer.Message;
}

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
		}
	
		return 1;
}


void ConnectWindow::mouse(int btn, int state, int x, int y) {

}

void ConnectWindow::keyboard(unsigned char c, int x, int y) {

}


