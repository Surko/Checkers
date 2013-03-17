#include "Game.h"

/*
Konstruktor pre hru. Nastavenie poctu kamenov na 0 a ostatnych 
private premennych v Game na pocitatocne hodnoty.
Nastavenie plochy podla metody getStonePositions kde sa vyuziva Positioning.
*/
Game::Game() {
	stones = 0;
	endGame = false;
	winStatus = 0;
	selectedX = -1;
	selectedY = -1;
	for (int x = 0; x < 8; ++x)
		for (int y = 0; y < 8; y++)
			board[x][y] = EMPTY;
	getStonePositions(BLACK, NORMAL, 0, 2);
	getStonePositions(WHITE, NORMAL, 6, 8);
}

/*
Metoda getStonePositions nastavi pozicie kamenov na ploche podla preurceneho rozlozenia.
Mozne doplnit o rozne rozlozenia alebo zmenit uz vytvorene.
*/
void Game::getStonePositions(Cell cell, Positioning type, int sy, int ey) {
	switch (type) {
	case NORMAL :
		for (int y = sy; y < ey; ++y)
			for (int x = (y+1) % 2; x < 8; x += 2)
				board[x][y] = cell;
		break;
	}
}

/*
Metoda cell ktora vracia hodnotu policka na poziciach x a y
zadanych ako parametre.
*/
Game::Cell Game::cell(int x, int y) const {
	return board[x][y];
}

/*
Metoda selectedCell vracia par s oznacenym polickom
*/
std::pair<int, int> Game::selectedCell() const {
	return std::make_pair(selectedX, selectedY);
}

/*
Metoda isMyTurn() vracia true/false ci je hrac na tahu.
*/
bool Game::isMyTurn() const {
	return myTurn;
}

/*
Metoda isSinglePlayer() vracia true/false podla toho ci sme v singleplayeri
alebo v multiplayeri.
*/
bool Game::isSinglePlayer() const {
	return single;
}

/*
Metoda setmode nastavuje mod hry (singleplayer alebo multiplayer) podla parametru
typu boolean. 
*/
void Game::setMode(bool single) {
	this->single = single;
}

/*
Metoda setSide nastavuje stranu hraca.
0 <=> false --> Cierna strana
1 <=> true --> Biela strana.
Metodou calcStones spocita pocet kamenov pre urcitu stranu.
*/
void Game::setSide(bool side) {

	this->side = side == 0 ? SIDE_BLACK : SIDE_WHITE;
	this->stones = calcStones((Cell)this->side);
				
	std::cout << stones << std::endl;
}

/*
Metoda calcStone spocita pocet kamenov pre urciteho hraca/stranu.
Vracia nam pocet kamenov.
*/
int Game::calcStones(Cell side) {
	int stones = 0;
	for (int x = 0; x < 8; x++) 
		for (int y = 0; y < 8; y++)
			if (board[x][y] == (Cell)side)
				stones++;
	return stones;
}

/*
Nastavuje ci je hrac na tahu podla parametru
*/
void Game::setTurn(bool turn) {
	this->myTurn = turn;
}

/*
Metoda oznaci policko na pozicii x a y zadanych ako parametre.
Testuje sa ci neoznacujeme mimo plochy alebo na policko obsahujuce cudzi kamen.
Pri nedodrzani testovacej podmienky necha oznacene pozicie na -1 => ziadne oznacenie.
*/
void Game::selectCell(int x, int y) {
	if ((x>=0)&&(x<8)&&(y<8)&&(y>=0)&&(board[x][y]==side)) {

		selectedX = x;
		selectedY = y;

	} else {

		selectedX = -1;
		selectedY = -1;

	}
}

/*
Metoda getLegalMoves vrati vsetky mozne legalne tahy pre hraca zadaneho parametrom Side (strana).
Vsetky taketo tahy navrati do parametru moves predavaneho referenciou. Legalne tahy su take ktore
idu do sikna o jedno policko s tym ze sa dodrzuje podmienka ze policko nie je mimo plochu a je prazdne.
Dalsimi legalnymi tahmi su take kde preskakujeme nepriatela. Testovacia podmienka je rovnaka.
*/
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

					if (board[xj][yj] == EMPTY) {
						Move move;
						move.push_back(Step(Pos(x,y),Pos(xj,yj)));
						moves.push_back(move);
					}

				}

			}				
}

/*
Metoda moveOther pohybuje nepriatelovymi kamenmi. Za touto metodou nie je ziadna inteligencia prepocitavanim
tahov. Na pohyb nam sluzi navrat moznych tahov a vybratie nejakeho nahodneho. 
Ked nie su ziadne legalne tahy tak ukonci hru a prepocita kto ma viac jednotiek a podla toho vyhodnoti vitazstvo.
Metoda sa vola iba pri singleplayeri.
*/
void Game::moveOther() {
	Side other = side == SIDE_WHITE ? SIDE_BLACK : SIDE_WHITE;
	std::vector<Move> legalMoves;
	getLegalMoves(other, legalMoves);

	if (legalMoves.empty()) {
		hasEnded();
		int x = calcStones((Cell)other);
		if (x < stones) {				
			setWinStatus(1);
		}
		if (x == stones) {			
			setWinStatus(-1);
		}
		if (x > stones) {
			setWinStatus(0);
		}
		
		selectedX = -1;
		selectedY = -1;
		return;
	}

	Move & move = legalMoves[rand() % legalMoves.size()];
	for (Move::iterator iter = move.begin(); iter != move.end(); iter++) {
		board[iter->first.first][iter->first.second] = EMPTY;
		board[iter->second.first][iter->second.second] = (Cell)other;

		// vymazanie toho co nepriatel preskocil
		if (abs(iter->second.first - iter->first.first) == 2)
			board[(iter->first.first + iter->second.first)/2][(iter->first.second + iter->second.second)/2] = EMPTY;

	}
	myTurn = true;

}

/*
Metoda move pohne nepriatelovym kamenom. Parametrami sx a sy urci odkial sa pohybuje => tam nastavi prazdne policko 
a parametrami ex a ey miesto kam sa kamen pohol. Podla strany hraca urci co za kamen je nepriatel.
*/
void Game::move(int sx, int sy, int ex, int ey) {

	board[sx][sy] = EMPTY;
	board[ex][ey] = (side == SIDE_WHITE ? BLACK : WHITE);

}

/*
Metoda decStones znizuje pocet hracovych kamenov.
*/
void Game::decStones() {
	stones--;
}

/*
Metoda del vymaze miesto na ploche. Nastavenie na EMPTY.
*/
void Game::del(int x, int y) {
	board[x][y] = EMPTY;
}

/*
Metoda move volana z GameWindow ktora ma za ulohu pohyb hracovych kamenov. V prvom rade zistime 
mozne legalne tahy. Ulozene su v premennej legalMoves. Ked neexistuje ziadne legalne tahy tak ked sme v multiplayeri
tak odposle spravu o ukonceni, pri singleplayeri urci vitazstvo podla toho ci ma hrac alebo PC viac kamenov.
Ked existuje legalny tah z miesta [selectedX,selectedY] na miesto [x,y] tak pohneme kamenom. V multiplayeri posielame spravu
s prikazom MOVE. Ked bol tah typu preskakovanie nepriatela tak este posleme spravu typu DELETE. 
Nakonci odoznacime vsetky kamene a poziadame o vykreslenie.
*/
void Game::move(int x, int y, std::vector<std::string> &msg) {

	std::vector<Move> legalMoves;
	getLegalMoves(side, legalMoves);

	if (legalMoves.empty()) {
		if (single) {
			Side other = side == SIDE_WHITE ? SIDE_BLACK : SIDE_WHITE;
			hasEnded();
			int x = calcStones((Cell)other);
			if (x < stones) {				
				setWinStatus(1);
			}
			if (x == stones) {			
				setWinStatus(-1);
			}
			if (x > stones) {
				setWinStatus(0);
			}
			return;
		} else {
			std::stringstream ss;
			ss << "FINAL " << stones;
			msg.push_back(ss.str());
			selectedX = -1;
			selectedY = -1;
			return;
		}
	}

	Step step = Step(Pos(selectedX, selectedY),Pos(x, y));

	for (std::vector<Move>::iterator iter = legalMoves.begin(); iter != legalMoves.end(); iter++) {
		if (iter->front() == step) {
			board[selectedX][selectedY] = EMPTY;
			board[x][y] = (Cell)side;
			std::stringstream ss;
			ss << "MOVE " << selectedX << " " << selectedY << " " << x << " " << y;					
			msg.push_back(ss.str());

			// vymazanie toho co sme preskocili
			if (abs(selectedX - x) == 2) {
				board[(selectedX + x) / 2][(selectedY + y) / 2] = EMPTY;
				std::stringstream ss;
				ss << "DELETE " << (selectedX + x) / 2 << " " << (selectedY + y) / 2;
				msg.push_back(ss.str());
			}
			selectedX = -1;
			selectedY = -1;
			myTurn = false;						
		}
	}
	
	selectedX = -1;
	selectedY = -1;
	return ;
}

/*
Metoda ukonci hru nastavenim premennej endGame na true.
*/
void Game::hasEnded() {
	endGame = true;
}

/*
Metoda isEnd vrati true/false podla toho ci je konec hry.
*/
bool Game::isEnd() {
	return endGame;
}

/*
Metoda getWinStatus vrati vitazny status. 0 - prehra, 1 - vyhra, -1 - remiza.
*/
int Game::getWinStatus() {
	return winStatus;
}

/*
Metoda setWinStatus nastavi vitazny status podla premennej status.
*/
void Game::setWinStatus(int status) {
	winStatus = status;
}

/*
Metoda getStones vrati pocet kamenov na ploche.
*/
int Game::getStones() {
	return stones;
}
