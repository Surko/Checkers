#include "Window.h"
#include "Tools.cpp"
#include <string>

struct Button {
public :
	const char * name;
	int minX,maxX,minY,maxY;
	void (*func)();
	Button& setName(const char * newname) {
		name = newname;
		return *this;
	}
	Button& setMinPos(int x, int y) {
		minX = x;
		minY = y;
		return *this;
	}
	Button& setMaxPos(int x, int y) {
		maxX = x;
		maxY = y;
		return *this;
	}
	Button& setFunction(void (*funct)() ) {
		func = funct;
		return *this;
	}

};

class StartWindow : public Window {

public :
	StartWindow();
	void display();
	void keyboard(unsigned char c, int x, int y);
	void mouse(int btn, int state, int x, int y);
	static void join();
	static void exit();

private : 
	Button buttons[2];
};