
#ifndef MINIMAX_H
#define MINIMAX_H

#include "board.h"
#include "cfig.h"
#include <climits>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <vector>
#include <utility>
#include <unordered_map>
#include <string>

namespace EngineConst
{
    const int WIN_SCORE = 1000000;
    const int LOSS_SCORE = -WIN_SCORE;

    const int INF = 1000000000;
    const int NEG_INF = -INF;

    const std::vector<int> LINE_SCORES = {0, 1, 10, 100, 1000, 10000};
};

class MinimaxAI
{
private:
	char playerMark, aiMark;
    int max_depth, time_limit, win_length;
    clock_t startTime;

	std::unordered_map<std::string, int> tt;

    bool undoMove(Board& board, const int& row, const int& col);

	int evaluateWindow(const int& pc, const int& oc, const int& openEnds);
	int evaluateWindowPlayer(const int& oc, const int& openEnds);
	int evaluateWindowAI(const int& pc, const int& openEnds);
	int evaluateCenter(const Board& board);
	int evaluateLines(const Board& board);
	
	bool isBoardEmpty(const Board& board);
	std::pair<int, int> getBoardCenter(const Board& board);
	void addFreeNeighs(
	    Board& board, int row, int col, std::vector<std::pair<int, int>>& moves, std::vector<std::vector<bool>>& seen
	);

	std::vector<std::pair<int, int>> collectNeighborMoves(Board& board);
	std::vector<std::pair<int, int>> generateCandidateMoves(Board& board);
	bool findKillMove(Board& board, const char& mark, std::pair<int, int>& move);

	bool scanKillMoves(Board& board, std::pair<int, int>& move);

	int rateMove(Board& board, const std::pair<int, int>& move);

	int getNeighScore(Board& board, const std::pair<int, int>& move);

	int getCenterScore(Board& board, const std::pair<int, int>& move);
	int Maximizing(
		Board& board, int depth, int& alpha, int& beta, const std::vector<std::pair<int, int>>& moves
	);
	int Minimizing(
		Board& board, int depth, int& alpha, int& beta, const std::vector<std::pair<int, int>>& moves
	);
	

	int heuristicEval(const Board& board);
    bool isTerminal(Board& board);
    
    int minimax(Board& board, int depth, int alpha, int beta, bool maximizing);
    
    void orderMoves(Board& board, std::vector<std::pair<int, int>>& moves);
    
    std::string serializeBoard(const Board& board);
    
    bool timeUp() const;

public:
	MinimaxAI(char aiMark, char playerMark, int max_depth, int time_limit, int win_length);
	MinimaxAI(const Cfig& cifg);
    std::pair<int, int> findBestMove(const Board& board);
};

#endif
