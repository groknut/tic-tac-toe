
#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "player.h"
#include "cfig.h"
#include "conio.h"

class Game
{
private:
	Board board;
	Player* player1;
	Player* player2;
	Player* currentPlayer;
	int win_length;
	bool gameOver;

	bool checkWin(const char& mark) const;
	bool checkWin() const;
	char getWinner() const;
	void switchPlayer();
	void printResult();

public:
	Game(const Cfig& config);
	~Game();

	void printBoard();
	void handleInput();
	void save();
	bool isOver();

	
};

#endif
