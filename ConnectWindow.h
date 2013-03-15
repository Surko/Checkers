#include "Window.h"
#include "Tools.cpp"
#include <string>

static const char * hostname = "127.0.0.1";
static const int port = 5000;

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
	std::string text[4];
	int wheelCount;
};