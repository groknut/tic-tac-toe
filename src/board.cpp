
#include "../head/board.h"

Board::Board(const int& bsize) : board_size(bsize)
{
	grid.resize(board_size * board_size, empty);
    
        
}

Board::Board(const int& bsize, const char& empty_symbol) : board_size(bsize), empty(empty_symbol)
{
	
	grid.resize(board_size * board_size, empty);
    
        
}

void Board::print(const char& player1, const char& player2) const
{

	
	    
    std::cout << std::endl << "   ";
    for (int i = 0; i < board_size; i++) {
        std::cout << std::setw(2) << i << " ";
    }
    std::cout << std::endl << "   ";
    for (int i = 0; i < board_size * 3; i++) {
        std::cout << "_";
    }
    std::cout << std::endl;
    
    for (int i = 0; i < board_size; i++) {
        std::cout << std::setw(2) << i << "| ";
        for (int j = 0; j < board_size; j++)
        {
        	if (grid[index(i, j)] == player1)
        		std::cout << colors::green << grid[index(i, j)];
        	else if (grid[index(i, j)] == player2)
        		std::cout << colors::red << grid[index(i, j)];
        	else
        		std::cout << grid[index(i, j)];

        	std::cout << colors::reset << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Board::print() const
{

	
	    
    std::cout << std::endl << "   ";
    for (int i = 0; i < board_size; i++) {
        std::cout << std::setw(2) << i << " ";
    }
    std::cout << std::endl << "   ";
    for (int i = 0; i < board_size * 3; i++) {
        std::cout << "_";
    }
    std::cout << std::endl;
    
    for (int i = 0; i < board_size; i++) {
        std::cout << std::setw(2) << i << "| ";
        for (int j = 0; j < board_size; j++)
        	std::cout << grid[index(i, j)] << "  ";        	
        std::cout << std::endl;
    }
    std::cout << std::endl;
}







bool Board::isCellEmpty(const int& row,const int& col) const 
{
	return row >= 0 && row < board_size && col >= 0 && col < board_size && grid[index(row, col)] == empty;
}

bool Board::isFull() const
{
    for (const auto& cell : grid)
        if (cell == empty) return false;
	
	
	
	
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
	grid[index(y, x)] = mark;
}

const char& Board::getCell(const int& y, const int& x) const
{
	if (y < 0 || y > board_size || x < 0 || x > board_size)
		throw GridError();
	return grid[index(y, x)];
}




std::vector<std::pair<int, int>> Board::getEmptyCells() const
{
	std::vector<std::pair<int, int>> cells;
	for (int i = 0; i < board_size; i++)
		for (int j = 0; j < board_size; j++)
			if (grid[index(i, j)] == empty)
				cells.emplace_back(i, j);
	return cells;
}

bool Board::checkWin(const char& mark, const int& win_length) const
{
    const int dirs[4][2] = {
        {0, 1},   
        {1, 0},   
        {1, 1},   
        {1, -1}  
    };
    
    for (int row = 0; row < board_size; row++) 
    {
        for (int col = 0; col < board_size; col++) 
        {
            if (grid[index(row, col)] != mark) continue;
            
            for (int d = 0; d < 4; d++) {
                int dr = dirs[d][0], dc = dirs[d][1];
                
                bool win = true;
                for (int k = 0; k < win_length; k++) {
                    int r = row + dr * k;
                    int c = col + dc * k;
                    if (r < 0 || r >= board_size || c < 0 || c >= board_size || grid[index(r, c)] != mark) 
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
