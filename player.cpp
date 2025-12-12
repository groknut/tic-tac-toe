
#include "player.h"

Player::Player(const char& sym, const std::string& n) 
    : mark(sym), name(n) {}

Human::Human(const char& sym, const std::string& n) 
    : Player(sym, n) {}

bool Human::makeMove(Board& board)
{
	int row, col;

	while (true)
	{
		std::cout << name << "(" << mark << "), input row and col (0-" << board.getSize() - 1 << "): ";

		if (!(std::cin >> row >> col))
		{
			std::cin.clear();
			std::cout << "Error: input 2 numbers" << std::endl;
			continue;
		}
		if (board.isCellEmpty(row, col))
		{
			board.setCell(row, col, mark);
			return true;
		}
		else
			std::cout << "Cell isn't empty!" << std::endl;
	}
}

AI::AI(const char& c, const std::string& n)
	: Player(c, n) {}

bool AI::makeMove(Board& board)
{
	for (int i = 0; i < board.getSize(); i++)
		for (int j = 0; j < board.getSize(); j++)
			if (board.isCellEmpty(i, j))
			{
				std::cout << name << "(" << mark << "), moves to (" << i << ", " << j << ")" << std::endl;
				board.setCell(i, j, mark);
				return true;
			}
	return false;
}
