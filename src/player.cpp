#include "../head/player.h"

Player::Player(const char& sym, const std::string& n) 
    : mark(sym), name(n) {}

Human::Human(const char& sym, const std::string& n) 
    : Player(sym, n) {}


bool Human::makeMove(Board& board)
{
	int row, col;
	bool validInput = false;

	while (!validInput)
	{
		std::cout << name << " (" << mark << "), input row and col (0-" << board.getSize() - 1 << "): ";
		        
        if (std::cin >> row >> col)
        {
            if (board.isCellEmpty(row, col))
            {
                board.setCell(row, col, mark);
                return true;
            }
            else
            {
                std::cout << "Cell isn't empty or out of bounds! Try again.\n";
            }
        }
        {
            std::cin.clear();
            std::cin.ignore(1000, '\n'); 
            std::cout << "Error: Please input two numbers!\n";
        }
    }
    return false;
}

AI::AI(const char& c, const std::string& n, const Cfig& cfig)
	: Player(c, n), aiEngine(cfig) 
{
    if (cfig.has("debug"))
        sleep_time = cfig.get<int>("debug", "sleep", 0);
}

bool AI::makeMove(Board& board) {
        
    auto bestMove = aiEngine.findBestMove(board);
    
    if (bestMove.first != -1 && bestMove.second != -1) 
    {
        std::cout << name << " (" << mark << ") moves to (" 
                  << bestMove.first << ", " << bestMove.second << ")\n";
        sleep(sleep_time);
            
        board.setCell(bestMove.first, bestMove.second, mark);
        std::cout << "AI move to " << "(" << bestMove.first << ", " << bestMove.second << std::endl;
        return true;
    }
    
    // Fallback
    for (int i = 0; i < board.getSize(); i++) 
    {
        for (int j = 0; j < board.getSize(); j++) 
        {
            if (board.isCellEmpty(i, j)) {
                std::cout << name << " (" << mark << "), moves to (" << i << ", " << j << ")" << std::endl;
                board.setCell(i, j, mark);
                return true;
            }
        }
    }
    
    return false;
}


void AI::sleep(int milliseconds) {
    clock_t start_time = clock();
    
    clock_t wait_time = milliseconds * CLOCKS_PER_SEC / 1000;
    
    while (clock() < start_time + wait_time) {}
}
