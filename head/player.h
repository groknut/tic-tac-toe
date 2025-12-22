#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"
#include <string>
#include <iostream>
#include "minimax.h"
#include "cfig.h"

class Game;

class Player
{
protected:
	char mark;
	std::string name;

public:
	Player(const char& c, const std::string& m);
	virtual ~Player() = default;

	virtual bool makeMove(Board& board) = 0;
	char getMark() const { return mark; }
	std::string getName() const { return name; }
};

class Human : public Player
{
public:
	Human(const char& c, const std::string& n);
	bool makeMove(Board& board) override;
};

class AI : public Player
{
private:
	MinimaxAI aiEngine;
    int sleep_time;
    std::vector<std::string> lastThinkingLog;
    void sleep(int milliseconds);
public:
	AI(const char& c, const std::string& n, const Cfig& cfig, Game* game = nullptr);
	bool makeMove(Board& board) override;
    void printThinkingLog();
    void clearLastLog() { lastThinkingLog.clear(); }
};

#endif
