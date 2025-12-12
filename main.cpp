#include <iostream>
#include "board.h"
#include "cfig.h"
int main()
{

	Cfig config(Cfig::EQUAL);
	config.load("config.ini");
	
	Board board(config.get<int>("board", "size", 3),config.get<char>("board", "empty", ' '));
	board.print();
	
    return 0;
}
