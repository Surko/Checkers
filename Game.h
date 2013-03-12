
#include <utility>
#include <vector>


typedef std::pair<int, int> Pos;
typedef std::pair<Pos, Pos> Step;
typedef std::vector<Step> Move;

class Game {
public :
	Game();
	
	enum Cell { EMPTY, BLACK = 1, WHITE = 2 };
	enum Side { SIDE_BLACK = BLACK , SIDE_WHITE = WHITE};
	Cell cell(int x, int y) const;
	std::pair<int, int> selectedCell() const;
	void selectCell(int x, int y);
	void move(int x, int y);
	void getLegalMoves(Side side, std::vector<Move> &moves) const;
	void getJumpMoves() const;
private :
	typedef Cell Board[8][8];
	Board board;
	int selectedX;
	int selectedY;
};