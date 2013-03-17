#pragma once
// Hladanie linkerom kniznicu Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")
#include <sdkddkver.h>

#include <WinSock2.h>
#include <Windows.h>
#include "glut.h"
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

// Definovane Windowy.
#define GAMEWINDOW 2
#define STARTWINDOW 1
#define CONNECTWINDOW 3
#define EXIT 0

using namespace std;

// Sirka a vyska displeju
static const int WIDTH = 600;
static const int HEIGHT = 600;

// Sirka vyska a odsadenie tlacidiel.
static const int BTN_WIDTH = 300;
static const int BTN_HEIGHT = 50;
static const int BTN_INSET = 20;

// Zaciatocne pozicie prveho tlacidla
static const int BTN_STARTX = 150;
static const int BTN_STARTY = (HEIGHT - BTN_HEIGHT) / 2;

// Polomer kamena
static const int radius = 30;

// Float na prevadzanie stupnov na radiany.
static const float DEG2RAD = 3.14159/180;

/*
Buffer - Stavba spravy ktora prichadza.
ID - id hraca/PC od ktoreho sprava prisla
Message - samotna sprava
*/
struct Buffer
{
	int ID;
	char Message[256];
};

class Window {
public : 
	virtual void display() = 0;
	virtual void keyboard(unsigned char c, int x, int y) = 0;
	virtual void mouse(int btn, int state, int x, int y) = 0;
	virtual void update() = 0;
	virtual std::string toString() = 0;
	static int STATE;
	static void changeState(bool side, bool single);
	static DWORD receiveMsg();
	static vector<string> msgQueue;
	static bool connected;
	static SOCKADDR_IN addr;
	static SOCKET sConnection;
	static int connID;	
};

