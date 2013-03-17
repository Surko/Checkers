#pragma once
#include <string>
#include <iostream>
#include "glut.h"

using namespace std;

/**
* Singleton trieda TextTools. Ma za ulohu vsetky operacie s textom.
* Pomocou metody getInstance ziskame instanciu TextTool. Ziskavame hu tak ze definujeme staticku
* premennu instance.
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

private :
	static Tools * instance;
};