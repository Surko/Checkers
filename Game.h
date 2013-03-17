
#include <utility>
#include <vector>
#include <sstream>
#include <iostream>

// typedef na poziciu. 2 suradnice v pare.
typedef std::pair<int, int> Pos;
// typedef na jeden krok. 2 pozicie v pare
typedef std::pair<Pos, Pos> Step;
// typedef pre mozne pohyby za jeden tah.
typedef std::vector<Step> Move;

/*
Hlavna trieda pre hru
*/
class Game {
public :
	Game();
	// Mozne hodnoty na ploche/board
	enum Cell { EMPTY, BLACK = 1, WHITE = 2 };
	// Dve mozne strany za ktore sa da hrat - priradene hodnoty z Cell pre lahku konverziu.
	enum Side { SIDE_BLACK = BLACK , SIDE_WHITE = WHITE};
	// Mozne druhy rozmiestnenia kamenov. Iba jedna. Mozne pridat dalsie pre ine druhy hier.
	enum Positioning { NORMAL };	
	Cell cell(int x, int y) const;
	std::pair<int, int> selectedCell() const;
	bool isMyTurn() const;
	bool isSinglePlayer() const;
	void setMode(bool single);
	void setSide(bool side);
	void setTurn(bool turn);
	void moveOther();
	void selectCell(int x, int y);
	void move(int x, int y, std::vector<std::string> &msg);
	void move(int sx, int sy, int ex, int ey);
	void del(int x, int y);
	void getLegalMoves(Side side, std::vector<Move> &moves) const;
	void getJumpMoves() const;
	void getStonePositions(Cell cell, Positioning type, int sy, int ey);
	void decStones();
	bool isEnd();
	void hasEnded();
	void setWinStatus(int status);
	int getWinStatus();
	int getStones();
	int calcStones(Cell side);
private :
	// typedef pre plochu. 8x8 miest s moznymi hodnotami WHITE,BLACK,EMPTY.
	typedef Cell Board[8][8];
	// Plocha na ktorej sa hra
	Board board;
	// Oznacene miesto x
	int selectedX;
	// Oznacene miesto y
	int selectedY;
	// Pocet kamenov
	int stones;
	// true/false ci som natahu
	bool myTurn;
	// true/false ci je konec hry
	bool endGame;
	// vitazny status.
	int winStatus;
	// true/false ci hrame single alebo multiplayer
	bool single;
	// Strana za ktoru hrame.
	Side side;
};