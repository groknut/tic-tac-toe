
#include "../head/board.h"

bool Board::undoMove(int row, int col) // ДОБАВЛЕНО
{
    if (row < 0 || row >= board_size || col < 0 || col >= board_size)
        return false;
    if (grid[row][col] == empty)
        return false;
    grid[row][col] = empty;
    return true;
}

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
	    
	    std::cout << std::endl << "   ";
	    for (int i = 0; i < size; i++) {
	        std::cout << std::setw(2) << i << " ";
	    }
	    std::cout << std::endl << "   ";
	    for (int i = 0; i < size * 3; i++) {
	        std::cout << "_";
	    }
	    std::cout << std::endl;
	    
	    for (int i = 0; i < size; i++) {
	        std::cout << std::setw(2) << i << "| ";
	        for (int j = 0; j < size; j++) {
	            std::cout << grid[i][j] << "  ";
	        }
	        std::cout << std::endl;
	    }
	    std::cout << std::endl;
}

const std::vector<std::vector<char>>& Board::getGrid() const 
{ 
	return grid; 
}


bool Board::isCellEmpty(const int& row,const int& col) const 
{
	return row >= 0 && row < board_size && col >= 0 && col < board_size && grid[row][col] == empty;
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
	if (y < 0 || y > board_size || x < 0 || x > board_size)
		throw GridError();
	grid[y][x] = mark;
}

const char& Board::getCell(const int& y, const int& x) const
{
	if (y < 0 || y > board_size || x < 0 || x > board_size)
		throw GridError();
	return grid[y][x];
}


std::vector<std::pair<int, int>> Board::getEmptyCells() const
{
	std::vector<std::pair<int, int>> cells;
	for (int i = 0; i < board_size; i++)
		for (int j = 0; j < board_size; j++)
			if (grid[i][j] == empty)
				cells.emplace_back(i, j);
	return cells;
}

bool Board::checkWin(const char& mark, const int& win_length) const
{
	const int dirs[4][2] = {
		{0, 1}, {1, 0}, {1, 1}, {1, -1}
	};
	    
    for (int row = 0; row < board_size; row++) 
    {
        for (int col = 0; col < board_size; col++) 
        {
            if (grid[row][col] != mark) continue;
            
            for (int d = 0; d < 4; d++) {
                int dr = dirs[d][0], dc = dirs[d][1];
                
                bool win = true;
                for (int k = 0; k < win_length; k++) {
                    int r = row + dr * k;
                    int c = col + dc * k;
                    if (r < 0 || r >= board_size || c < 0 || c >= board_size || grid[r][c] != mark) 
                    {
                        win = false;
                        break;
                    }
                }
                if (win) 
                	return true;
            }
        }
    }
    return false;
}
