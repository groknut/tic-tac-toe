#include <iostream>
#include "./head/game.h"
#include "cfig.h"
int main()
{

	Cfig config("config.ini", Cfig::EQUAL, Cfig::HASH);

	Game game(config);

    game.run();

    // do
	// {
	// 	game.printBoard();
	// 	game.handleInput();
 	// } while (!game.isOver());
 	
    return 0;
}
