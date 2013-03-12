#include "Window.h"
#include "Tools.cpp"
#include <string>

class ConnectWindow : public Window {

public :
	ConnectWindow();
	void display();
	void keyboard(unsigned char c, int x, int y);
	void mouse(int btn, int state, int x, int y);

private :
	std::string text[4];
	int wheelCount;
};