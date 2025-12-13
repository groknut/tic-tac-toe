#include <iostream>
#include "game.h"
#include "cfig.h"
int main()
{

	Cfig config(Cfig::EQUAL);
	config.load("config.ini");

	Game game(config);

    std::cout << "\n=== Tic-Tac-Toe ===\n";

    do
	{
		game.printBoard();
		game.handleInput();
 	} while (!game.isOver());

 	// game.printBoard();

    return 0;
}
