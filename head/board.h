
#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

namespace colors
{
	const std::string red = "\u001b[91m";
	const std::string green = "\u001b[92m";
	const std::string reset = "\u001b[0m";
}

class Board
{
private:
	std::vector<std::vector<char>> grid;
	char empty = ' ';
	int board_size = 3;

public:
	Board() {};
	Board(const int& bsize);
	Board(const int& bsize, const char& empty_symbol);
	void print() const;
	void print(const char& player1, const char& player2) const;
	bool isCellEmpty(const int& row,const int& col) const;

	bool isFull() const;
	char getEmpty() const { return empty; }	    
    int getSize() const;
    void setCell(const int& y, const int& x, const char& mark);
	const char& getCell(const int& y, const int& x) const;
    const std::vector<std::vector<char>>& getGrid() const;

	bool checkWin(const char& mark, const int& win_length) const;
	
    std::vector<std::pair<int, int>> getEmptyCells() const;
    bool undoMove(int row, int col);
};

class GridError {};

#endif
