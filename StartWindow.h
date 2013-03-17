#include "Window.h"
#include "Tools.cpp"
#include <string>

static const int BUTTON_COUNTER = 3;

/*
Struktura pre tlacidlo.
name - napis na tlacidle.
min/max - minimalne maximalne pozicie x a y
func - funkcia na zavolanie pri stlaceni.
*/ 
struct Button {
public :
	const char * name;
	int minX,maxX,minY,maxY;
	void (*func)();

	// Nastavenie napisu.
	Button& setName(const char * newname) {
		name = newname;
		return *this;
	}

	// Nastavenie minimalnych pozic.
	Button& setMinPos(int x, int y) {
		minX = x;
		minY = y;
		return *this;
	}

	// Nastavenie maximalnych pozic.
	Button& setMaxPos(int x, int y) {
		maxX = x;
		maxY = y;
		return *this;
	}

	// Nastavenie funkcie.
	Button& setFunction(void (*funct)() ) {
		func = funct;
		return *this;
	}

};

/*
Zdedena trieda od Window. StartWindow - zaciatocne okno pri spusteni.
*/
class StartWindow : public Window {

public :
	StartWindow();
	void display();
	void keyboard(unsigned char c, int x, int y);
	void mouse(int btn, int state, int x, int y);
	void update();
	string toString();
	static void join();
	static void exit();
	static void create();

private : 
	// Tlacidla v StartWindow.
	Button buttons[BUTTON_COUNTER];
};