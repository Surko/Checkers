#include "Game.h"

Game::Game() {
	selectedX = -1;
	selectedY = -1;
	for (int x = 0; x < 8; ++x)
		for (int y = 0; y < 8; y++)
			board[x][y] = EMPTY;
	for (int y = 0; y < 2; ++y)
		for (int x = (y+1) % 2; x < 8; x += 2)
			board[x][y] = BLACK;
	for (int y = 6; y < 8; ++y)
		for (int x = (y+1) % 2; x < 8; x += 2)
			board[x][y] = WHITE;
}

Game::Cell Game::cell(int x, int y) const {
	return board[x][y];
}

std::pair<int, int> Game::selectedCell() const {
	return std::make_pair(selectedX, selectedY);
}

bool Game::isMyTurn() const {
	return myTurn;
}

bool Game::isSinglePlayer() const {
	return single;
}

void Game::setMode(bool single) {
	this->single = single;
}

void Game::selectCell(int x, int y) {
	if ((x>=0)&&(x<8)&&(y<8)&&(y>=0)&&(board[x][y]==WHITE)) {

		selectedX = x;
		selectedY = y;

	} else {

		selectedX = -1;
		selectedY = -1;

	}
}

void Game::getLegalMoves(Side side, std::vector<Move> &moves) const {

	for (int x = 0; x < 8; x++)
		for (int y = 0;y < 8;y++)
			if (board[x][y] == (Cell)side) {
				int yPos = y+ (side == SIDE_WHITE ? -1 : 1);
				if (yPos < 0) continue;
				if (yPos >= 8) continue;
				for (int xPos = x-1; xPos <= x+1; xPos+=2) {
					if (xPos < 0) continue;
					if (xPos >= 8) continue;
					if (board[xPos][yPos] == EMPTY) {
						Move move;
						move.push_back(Step(Pos(x,y),Pos(xPos,yPos)));
						moves.push_back(move);
					}

				}

				const int dx[] = {-1, 1, 1, -1};
				const int dy[] = {-1, -1, 1, 1};

				for (int d = 0; d < 4; d++) {
					const int xx = x + dx[d];
					const int yy = y + dy[d];

					if (xx < 0 || xx >=8) continue;
					if (yy >= 8 || yy >=8) continue;
					if (board[xx][yy] == EMPTY || board[xx][yy] == (Cell)side) continue;

					// Jumped x and y positions
					const int xj = x + 2 * dx[d];
					const int yj = y + 2 * dy[d];

					if (xj < 0 || xj >=8) continue;
					if (yj < 0 || yj >=8) continue;

					if (board[xx][yy] == EMPTY) {
						Move move;
						move.push_back(Step(Pos(x,y),Pos(xj,yj)));
						moves.push_back(move);
					}

				}

			}
				
}

void Game::moveBlack() {



}

void Game::move(int x, int y) {

	std::vector<Move> legalMoves;
	getLegalMoves(SIDE_WHITE, legalMoves);

	Step step = Step(Pos(selectedX, selectedY),Pos(x, y));

	for (std::vector<Move>::iterator iter = legalMoves.begin(); iter != legalMoves.end(); iter++) {
		if (iter->front() == step) {
			board[selectedX][selectedY] = EMPTY;
			board[x][y] = WHITE;
			if (abs(selectedX - x) == 2)
				board[(selectedX + x) / 2][(selectedY + y) / 2] = EMPTY;

			myTurn = false;
			
		}
	}
	
	selectedX = -1;
	selectedY = -1;

}
