
#include "../head/game.h"

Game::Game(const Cfig& config)
{
	win_length = config.get<int>("game", "win_length", 3);
	board = Board(config.get<int>("board", "size", 3),config.get<char>("board", "empty", ' '));

	if (config.get("player2", "type").toString() == std::string("ai"))
	{
		player1 = new Human(config.get<char>("player1", "mark", 'X'), config.get<std::string>("player1", "name", "Player"));
		player2 = new AI(
			config.get<char>("player2", "mark", 'O'), 
			config.get<std::string>("player2", "name", "AI")
		);
	}
	else
	{
		player1 = new Human(config.get<char>("player1", "mark", 'X'), config.get<std::string>("player1", "name", "Player1"));
		player2 = new Human(config.get<char>("player2", "mark", 'O'), config.get<std::string>("player2", "name", "Player2"));
	}

	// player1->setOpponentMark(player2->getMark());
	// player2->setOpponentMark(player1->getMark());

	currentPlayer = player1;
    gameOver = false;
}

Game::~Game()
{
    delete player1;
    delete player2;
}

void Game::printBoard()
{
	system("cls");
    board.print();
}

bool Game::isOver()
{
    return gameOver;
}

void Game::switchPlayer()
{
    if (currentPlayer == player1)
        currentPlayer = player2;
    else
        currentPlayer = player1;
}

char Game::getWinner() const
{
    if (checkWin(player1->getMark())) return player1->getMark();
    if (checkWin(player2->getMark())) return player2->getMark();
    return ' ';
}

bool Game::checkWin(const char& mark) const
{
	int size = board.getSize();

	const int dirs[4][2] = {
		{0, 1}, {1, 0}, {1, 1}, {1, -1}
	};

	for (int row = 0; row < size; row++)
	{
		for (int col = 0; col < size; col++)
		{
			if (board.getCell(row, col) != mark)
				continue;

			for (int d = 0; d < 4; d++)
			{
				int dr = dirs[d][0], dc = dirs[d][1];

				int end_row = row + dr * (win_length - 1);
				int end_col = col + dc * (win_length - 1);

				if (end_row < 0 || end_row >= size || end_col < 0 || end_col >= size)
					continue;

				bool win = true;

				for (int k = 0; k < win_length; k++)
				{
					if (board.getCell(row + dr * k, col + dc * k) != mark)
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

bool Game::checkWin() const
{
    return checkWin(player1->getMark()) || checkWin(player2->getMark());
}

void Game::printResult()
{
    char winner = getWinner();
    if (winner != ' ')
    {
        std::string winnerName = (winner == player1->getMark()) ? player1->getName() : player2->getName();
        std::cout << "Congratulations " << winnerName << "!\n";
    }
}



void Game::handleInput()
{
    
    std::cout << currentPlayer->getName() << "'s turn (" << currentPlayer->getMark() << "):\n";
    
    bool moveMade = currentPlayer->makeMove(board);

	if (moveMade)
	{
		if (checkWin())
		{
			gameOver = true;
			std::cout << "\n" << currentPlayer->getName() << " (" << currentPlayer->getMark() << ") wins!\n";
            printResult();
		}
		else if (board.isFull())
		{
			gameOver = true;
			std::cout << "\nIt's a draw!\n";
			printResult();
		}
		else
			switchPlayer();
	}
    
    else
        std::cout << "Invalid move, try again.\n";
}
