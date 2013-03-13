#pragma once
#include "Window.h"
#include "Game.h"
#include "Tools.cpp"

class GameWindow : public Window {

public :
	GameWindow(bool single);
	Game * getGame();
	void display();
	void keyboard(unsigned char c, int x, int y);
	void mouse(int btn, int state, int x, int y);
private : 
	Game game;
};