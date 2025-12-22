
#include "../head/game.h"

Game::Game(const Cfig& config)
{
	win_length = config.get<int>("game", "win_length", 3);
	board = Board(config.get<int>("board", "size", 3),config.get<char>("board", "empty", ' '));

	if (config.get("player2", "type").toString() == std::string("ai") && config.get("player1", "type").toString() == std::string("human"))
	{
		player1 = new Human(config.get<char>("player1", "mark", 'X'), config.get<std::string>("player1", "name", "Player"));
		player2 = new AI(
			config.get<char>("player2", "mark", 'O'), 
			config.get<std::string>("player2", "name", "AI"),
			config, this
		);
	}
	else if (config.get("player1", "type").toString() == std::string("human") && config.get("player2", "type").toString() == std::string("human"))
	{
		player1 = new Human(config.get<char>("player1", "mark", 'X'), config.get<std::string>("player1", "name", "Player1"));
		player2 = new Human(config.get<char>("player2", "mark", 'O'), config.get<std::string>("player2", "name", "Player2"));
	}
    else
    {
        player1 = new AI(
			config.get<char>("player1", "mark", 'O'), 
			config.get<std::string>("player1", "name", "AI"),
			config, this
		);
        player2 = new AI(
			config.get<char>("player2", "mark", 'O'), 
			config.get<std::string>("player2", "name", "AI"),
			config, this
		);
    }

    playerMark = player1->getMark();
    aiMark = player2->getMark();

	color = config.get<bool>("game", "color", false);

    if (config.has("debug"))
        clear_console = config.get<bool>("debug", "clear_console", false);

	if (config.get("game", "start").toChar() == player1->getMark())
		currentPlayer = player1;
	else
		currentPlayer = player2;

    gameOver = false;
}

Game::~Game()
{
    delete player1;
    delete player2;
}

void Game::printBoard()
{
    if (clear_console)
    	system("cls");
	if (color)
		board.print(player1->getMark(), player2->getMark());
    else
    	board.print();

    if (currentPlayer != nullptr) 
    {
        Player* previousPlayer = (currentPlayer == player1) ? player2 : player1;
        
        AI* previousAI = dynamic_cast<AI*>(previousPlayer);
        if (previousAI) {
            previousAI->printThinkingLog();
            previousAI->clearLastLog();
        }
    }
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
    if (checkWin(playerMark)) return playerMark;
    if (checkWin(aiMark)) return aiMark;
    return ' ';
}

bool Game::checkWin(const char& mark) const
{
    return board.checkWin(mark, win_length);
}

bool Game::isTerminal(const Board& board) const 
{
    return board.checkWin(aiMark, win_length) ||
           board.checkWin(playerMark, win_length) ||
           board.isFull();
}

bool Game::checkWin() const
{
    return checkWin(playerMark) || checkWin(aiMark);
}

void Game::printResult()
{
    char winner = getWinner();
    if (winner != ' ')
    {
    	printBoard();
        std::string winnerName = (winner == playerMark) ? player1->getName() : player2->getName();
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


void Game::run()
{
    do
	{
		printBoard();
		handleInput();
 	} while (!isOver());
}



int Game::evaluate(const Board& board) const
{
    if (board.checkWin(aiMark, win_length))
        return EngineConst::WIN_SCORE;
    else if (board.checkWin(playerMark, win_length))
        return EngineConst::LOSS_SCORE;
   
    return evaluatePosition(board);
}

int Game::evaluateLine(const Board& board, std::pair<int, int> start, std::pair<int, int> delta) const 
{    
    int aiCount = 0, playerCount = 0, emptyCount = 0;
    int size = board.getSize();
    char emptyCell = board.getEmpty();
    auto [startRow, startCol] = start;
    auto [deltaRow, deltaCol] = delta;
   
    for (int i = 0; i < win_length; i++) 
    {
        int row = startRow + i * deltaRow;
        int col = startCol + i * deltaCol;
        char cell = board.getCell(row, col);
        
        if (cell == aiMark)
            aiCount++;
        else if (cell == playerMark)
            playerCount++;
        else if (cell == emptyCell)
            emptyCount++;
    }
    
    if (aiCount > 0 && playerCount > 0)
        return 0;
    
    if (aiCount > 0 && playerCount == 0) {
        int baseScore = EngineConst::LINE_SCORES[aiCount];

        if (emptyCount > 0)
            return baseScore + (emptyCount * 2);
        return baseScore;
    }
    
    if (playerCount > 0 && aiCount == 0) {
        int baseScore = EngineConst::LINE_SCORES[playerCount];
        
        if (emptyCount > 0)
            return -baseScore + (emptyCount * 2);
        return -baseScore;
    }
    
    return 0;
}


int Game::evaluatePosition(const Board& board) const 
{
    int size = board.getSize();
    int score = 0;
   
    for (int row = 0; row < size; row++)
        for (int col = 0; col <= size - win_length; col++)
            score += evaluateLine(board, {row, col}, {0, 1});
    
    for (int col = 0; col < size; col++)
        for (int row = 0; row <= size - win_length; row++)
            score += evaluateLine(board, {row, col}, {1, 0});
    
    for (int row = 0; row <= size - win_length; row++)
        for (int col = 0; col <= size - win_length; col++)
            score += evaluateLine(board, {row, col}, {1, 1});
       
    for (int row = 0; row <= size - win_length; row++)
        for (int col = win_length - 1; col < size; col++)
            score += evaluateLine(board, {row, col}, {1, -1});
    
   
    int center = size / 2;
    for (int i = center - 1; i <= center + 1; i++)
        for (int j = center - 1; j <= center + 1; j++)
            if (i >= 0 && i < size && j >= 0 && j < size) 
            {
                if (board.getCell(i, j) == aiMark)
                    score += 5;
                else if (board.getCell(i, j) == playerMark)
                    score -= 5;
            }
    
    return score;
}
