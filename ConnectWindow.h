#include "Window.h"
#include "Tools.cpp"
#include <string>

// Host na koreho sa napajame, mozne zmenit na nacitavanie z txt suboru
// alebo nacitanie priamo z hry.
static const char * hostname = "127.0.0.1";
// Port na ktorom hra bezi
static const int port = 5000;

/*
Zdedena trieda od Window. ConnectWindow - okno pri napajani na server.
*/
class ConnectWindow : public Window {

public :
	ConnectWindow();
	void display();
	void keyboard(unsigned char c, int x, int y);
	void mouse(int btn, int state, int x, int y);
	void update();
	int tryConnect();
	string toString();
private :
	// Pole pre text ktory sa zobrazuje na obrazovke pri napajani alebo cakani na server.
	std::string text[4];
	// Stav ktory text vypisujeme
	int wheelCount;
	// Strana za ktoru budeme hrat.
	int side;
};