#ifndef _MINIMAX_H_
#define _MINIMAX_H_

#include "cfig.h"
#include "board.h"

#include "engine_const.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <algorithm>

class Game;

class MinimaxAI
{
private:
    char playerMark;   
    char aiMark;       
    int max_depth;     
    int time_limit;    
    int win_length;    
    clock_t startTime; 
    bool showThinking; 

    std::vector<std::string> thinkingLog;

    Game* game;   
    
    bool timeUp() const;
    
    void setMove(Board& board, int row, int col, char mark);
    void undoMove(Board& board, int row, int col);
    
    int minimax(Board& board, int depth, int alpha, int beta, bool maximizingPlayer);
    
    std::pair<int, int> iterativeDeepening(Board& board);
    
    std::vector<std::pair<int, int>> getOrderedMoves(const Board& board, bool forAI) const;
    
    int quickMoveScore(const Board& board, int row, int col, char mark, bool forAI) const;
    
    bool isWinningMove(const Board& board, int row, int col, char mark) const;
public:
    
    MinimaxAI() {};
    MinimaxAI(const Cfig& cfig, Game* game = nullptr);
    ~MinimaxAI();

    std::pair<int, int> findBestMove(Board& board);
    const std::vector<std::string>& getThinkingLog() const { return thinkingLog; }
    void clearThinkingLog() { thinkingLog.clear(); }
};

#endif
