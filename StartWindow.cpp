#include "StartWindow.h"
 

/*
Konstruktor pre StartWindow. Nastavi meno a funkciu pre tlacidlo.
*/ 
StartWindow::StartWindow() {			
		buttons[0] = Button();
		buttons[0].setName("Join Server").setFunction(&StartWindow::join);
		buttons[2] = Button();
		buttons[2].setName("Exit").setFunction(&StartWindow::exit);
		buttons[1] = Button();
		buttons[1].setName("Single Player").setFunction(&StartWindow::create);
}

/*
Display funkcia pre StartWindow. Sklada sa z vykreslenia troch tlacidiel 
podla Button struktury zadefinovanej v StartWindow.h
*/
void StartWindow::display() {
	glClear(GL_COLOR_BUFFER_BIT);		
	Tools * tool = Tools::getInstance();
		
	for (int i = 0; i < 3; i++) {
		float x = BTN_STARTX;
		float y = BTN_STARTY + (i * (BTN_INSET + BTN_HEIGHT));
			glColor3f(0.7f,0.7f,0.7f);
		glBegin(GL_POLYGON);
		glVertex2f(x , y);
		glVertex2f(x + BTN_WIDTH, y);	
		glVertex2f(x + BTN_WIDTH, y + BTN_HEIGHT);
		glVertex2f(BTN_STARTX , y + BTN_HEIGHT);
		glEnd();
		
		glBegin(GL_POLYGON);	
		glColor3f(0.4f,0.4f,0.4f);
		glVertex2f(x + 2 , 2 + y);
		glVertex2f(x + BTN_WIDTH, 2 + y);	
		glVertex2f(x + BTN_WIDTH, y + BTN_HEIGHT);
		glVertex2f(x + 2, y + BTN_HEIGHT);
		glEnd();
			buttons[i].setMinPos(x,y).setMaxPos(x + BTN_WIDTH, y + BTN_HEIGHT);
			tool->displayText(x + BTN_WIDTH / 2,  y + BTN_HEIGHT / 2, 
			50,0,0,buttons[i].name);
	}
		glutSwapBuffers();
}

/*
Prazdna update funkcia. Mozne doplnit v buducnosti.
*/
void StartWindow::update() {

}

/*
Prazdna keyboard funkcia. Mozne doplnit v buducnosti.
*/
void StartWindow::keyboard(unsigned char c, int x, int y) {
}

/*
Mouse funkcia ktora spracovava eventy vykonane na tlacidlach => 
zavola funkciu prisluchajucu k tlacidlu.
*/
void StartWindow::mouse(int btn, int state, int x, int y) {
if (state == GLUT_UP) {
	for (Button butt : buttons) {				
		if (x <= butt.maxX && x >= butt.minX && y <= butt.maxY && y >= butt.minY) {
			butt.func();
			glutPostRedisplay();
		}
	}
}
}
	
/*
Funkcia Join. Jedna z funkcii ktora sa da priradit tlacidlu. Pri zavolani
do konzoli vypise co za funkciu sa vykona a nastavi prislusny stav a 
zavola zmenenie stavu. Funkcia zmeni na ConnectWindow odkial sa pripaja na server.
*/
void StartWindow::join() {
	std::cout << "Button : Join" << std::endl;
	STATE = 3;
	
	changeState(0, false);
}

/*
Funkcia Exit. Jedna z funkcii ktora sa da priradit tlacidlu. Pri zavolani
do konzoli vypise co za funkciu sa vykona a nastavi prislusny stav a 
zavola zmenenie stavu. Funkcia ukonci hru.
*/
void StartWindow::exit() {
	std::cout << "Button : Exit" << std::endl;
	STATE = 0;
	changeState(0, false);
}

/*
Funkcia Create. Jedna z funkcii ktora sa da priradit tlacidlu. Pri zavolani
do konzoli vypise co za funkciu sa vykona a nastavi prislusny stav a 
zavola zmenenie stavu. Funkcia zmeni na GameWindow s tym ze sme v SinglePlayer mode.
*/
void StartWindow::create() {
	std::cout << "Button : Create" << std::endl;
	STATE = 2;
	changeState(1, true);
}

/*
Funkcia to string vracia string s menom triedy.
*/
string StartWindow::toString() {
	return string("Start Window");
}