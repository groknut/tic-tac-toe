
#ifndef _BOARD_H_
#define _BOARD_H_

#include <iostream>
#include <iomanip>
#include <vector>

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
	bool isCellEmpty(const int& row,const int& col) const;

	bool isFull() const;
	    
    int getSize() const;
    void setCell(const int& y, const int& x, const char& mark);
	const char& getCell(const int& y, const int& x) const;
    const std::vector<std::vector<char>>& getGrid() const;

};

class GridError {};

#endif
