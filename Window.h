#pragma once

#include "glut.h"
#include <iostream>

#define GAMEWINDOW 2
#define STARTWINDOW 1
#define CONNECTWINDOW 3
#define EXIT 0

static const int WIDTH = 600;
static const int HEIGHT = 600;

static const int BTN_WIDTH = 300;
static const int BTN_HEIGHT = 50;
static const int BTN_INSET = 20;

static const int BTN_STARTX = 150;
static const int BTN_STARTY = (HEIGHT - BTN_HEIGHT) / 2;

static const int radius = 30;

static const float DEG2RAD = 3.14159/180;

class Window {
public : 
	virtual void display() = 0;
	virtual void keyboard(unsigned char c, int x, int y) = 0;
	virtual void mouse(int btn, int state, int x, int y) = 0;

	static int STATE;
	static void changeState(bool single);
};

