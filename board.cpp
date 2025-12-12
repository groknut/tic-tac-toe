
#include "board.h"

Board::Board(const int& bsize) : board_size(bsize)
{
	grid.resize(board_size);
    for (auto& row : grid)
        row.resize(board_size, empty);
}

Board::Board(const int& bsize, const char& empty_symbol) : board_size(bsize), empty(empty_symbol)
{
	grid.resize(board_size);
    for (auto& row : grid)
        row.resize(board_size, empty);
}

void Board::print() const
{

	int size = grid.size();
	    
	    // Заголовок
	    std::cout << "\n   ";
	    for (int i = 0; i < size; i++) {
	        std::cout << std::setw(2) << i << " ";
	    }
	    std::cout << std::endl << "   ";
	    for (int i = 0; i < size * 3; i++) {
	        std::cout << "_";
	    }
	    std::cout << std::endl;
	    
	    // Данные
	    for (int i = 0; i < size; i++) {
	        std::cout << std::setw(2) << i << "| ";
	        for (int j = 0; j < size; j++) {
	            std::cout << grid[i][j] << "  ";
	        }
	        std::cout << std::endl;
	    }
	    std::cout << std::endl;
	
	// for (size_t i = 0; i < grid.size(); i++)
	// {
	// 	std::cout << i << '|';
	// 	for (size_t j = 0; j < grid[i].size(); j++)
	// 		std::cout << grid[i][j] << '|';
	// 	std::cout << std::endl;
	// }
	// std::cout << std::endl;
}

const std::vector<std::vector<char>>& Board::getGrid() const 
{ 
	return grid; 
}


bool Board::isCellEmpty(const int& row,const int& col) const 
{
	return (size_t)row >= 0 && (size_t)row < grid.size() && (size_t)col >= 0 && (size_t)col < grid.size() && grid[row][col] == empty;
}

bool Board::isFull() const
{
	for (const auto& row : grid)
		for (const char& cell : row)
			if (cell == empty)
				return false;
	return true;
}

int Board::getSize() const
{
	return board_size;
}

void Board::setCell(const int& y, const int& x, const char& mark)
{
	if (y < 0 || (size_t)y > grid.size() || x < 0 || (size_t)x > grid.size())
		throw GridError();
	grid[y][x] = mark;
}

const char& Board::getCell(const int& y, const int& x) const
{
	if (y < 0 || (size_t)y > grid.size() || x < 0 || (size_t)x > grid.size())
		throw GridError();
	return grid[y][x];
}
