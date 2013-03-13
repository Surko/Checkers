#pragma once
// Hladanie linkerom kniznicu Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#include <sdkddkver.h>

#include <WinSock2.h>
#include <Windows.h>

#include <string>
#include <iostream>
#include "glut.h"

using namespace std;

static const char * hostname = "127.0.0.1";
static const int port = 5000;
/**
* Singleton trieda TextTools. Ma za ulohu vsetky operacie s textom.
* Pomocou metody getInstance ziskame instanciu TextTool. Ziskavame hu tak ze definujeme staticku
* premennu instance. Vyhoda tohoto je ze object nie je vytvoreny do zavolania tejto metody.
* Keby bola definovana v private bloku staticka premenna tak ako vieme staticke premenne
* sa vytvaraju pri zapnuti hry.
*/
class Tools {

public :
	/**
	* Metoda getInstance vrati odkaz na objekt typu TextTool.
	*/
	static Tools * getInstance() {	
		return instance;
		
	}

	/**
	* Jedna z textovych metod. Tato ma za ulohu zobrazit text na obrazovku.
	* Text sa zobrazi v bode [x,y] zadanom parametrami. Farba textu je urcena dalsimi troma parametrami
	* [r,g,b]. Samotny text na zobrazenie je predany parametrom string a zobrazujeme
	* ho pomocou glut metody glutBitmapCharacter
	*/ 
	void displayText( float x, float y, int r, int g, int b, const char *string ) {
		int j = strlen( string );
	
		glColor3f( r, g, b );
		glRasterPos2f( x, y );
		for( int i = 0; i < j; i++ ) {
			glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, string[i] );
		}
	}

	int tryConnect() {
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
	
		SOCKADDR_IN addr;
		int adrlen = sizeof(addr);
	
		SOCKET sConnection = socket(AF_INET, SOCK_STREAM, NULL);
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
			int ID;
			char * cID = new char[64];
			ZeroMemory(cID, 64);
	
			recv(sConnection, cID, 64, NULL);
			ID = atoi(cID);
			printf("You are client No:%d", ID);
		}
	
		return 1;
	}

private :
	static Tools * instance;
	static SOCKADDR_IN addr;
	static SOCKET sConnect;
};