
#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "player.h"
#include "cfig.h"
#include "engine_const.h"


class Game
{
private:
	Board board;
	Player* player1;
	Player* player2;
	Player* currentPlayer;
	int win_length;
	bool gameOver;
	bool color;
      bool clear_console;

    char aiMark;
    char playerMark;

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
	bool isOver();
    void run();

    int evaluate(const Board& board) const;
    int evaluatePosition(const Board& board) const;
    int evaluateLine(const Board& board, std::pair<int, int> start, std::pair<int, int> delta) const;

    bool isTerminal(const Board& board) const;
};

#endif
