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
	if (!connected) {
		if (tryConnect()) {
		connected = true;			
		} else {
			// Ked sa nepodari tak zavola vykreslenie podla metody display v tomto connectWindow.
			// ziskaj spravu ked sme napojeny
			// prekladanie spravy
			glutPostRedisplay();
		}
	} else {				
		if (!msgQueue.empty()) {	
			// zmenenie na GameWindow	
			STATE = 2;
			string msg = msgQueue.back();
			msgQueue.pop_back();
			cout << msg << endl;
			istringstream iss(msg);			
			iss >> msg;
			iss >> msg;
			bool side = atoi(msg.c_str());
			cout << "Your side: " << (side == true ? "WHITE" : "BLACK") << endl;
			// Je to multiplayer takze parameter v changeState bude false
			changeState(side, false);
			glutPostRedisplay();
		}
	}
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
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) receiveMsg, NULL, NULL, NULL);
		}
		
		return 1;
}


void ConnectWindow::mouse(int btn, int state, int x, int y) {

}

void ConnectWindow::keyboard(unsigned char c, int x, int y) {

}


string ConnectWindow::toString() {
	return string("Connect Window");
}