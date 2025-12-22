#include "../head/minimax.h"
#include "../head/game.h"

MinimaxAI::MinimaxAI(const Cfig& cfig, Game* gameptr) : game(gameptr)
{   
    aiMark = cfig("player2", "mark").toChar();     
    playerMark = cfig("player1", "mark").toChar(); 
    max_depth = cfig("AI", "depth").toInt();       
    time_limit = cfig("AI", "time_limit").toInt(); 
    win_length = cfig("game", "win_length").toInt();
    showThinking = cfig.get<bool>("AI", "show_thinking", false);    
}

MinimaxAI::~MinimaxAI()
{
    delete game;
}

bool MinimaxAI::timeUp() const 
{   
    return (clock() - startTime) * 1000.0 / CLOCKS_PER_SEC > time_limit;
}

void MinimaxAI::setMove(Board& board, int row, int col, char mark) 
{
    board.setCell(row, col, mark);
}

void MinimaxAI::undoMove(Board& board, int row, int col) 
{
    board.setCell(row, col, board.getEmpty());
}

int MinimaxAI::quickMoveScore(const Board& board, int row, int col, char mark, bool forAI) const 
{
    int score = 0;
    int size = board.getSize();
    int center = size / 2;
    int distance = abs(row - center) + abs(col - center);
    score += (size - distance) * 3;

    Board tempBoard = board;
    tempBoard.setCell(row, col, mark);
    if (tempBoard.checkWin(mark, win_length))
        score += EngineConst::WIN_SCORE / 10;

    char opponentMark = forAI ? playerMark : aiMark;
    tempBoard.setCell(row, col, opponentMark);
    if (tempBoard.checkWin(opponentMark, win_length))
        score += EngineConst::WIN_SCORE / 20;
    
    return score;
}

std::vector<std::pair<int, int>> MinimaxAI::getOrderedMoves(const Board& board, bool forAI) const 
{    
    std::vector<std::pair<int, int>> moves;   
    int size = board.getSize();
    std::vector<bool> considered(size * size, false);

    for (int i = 0; i < size; i++) 
        for (int j = 0; j < size; j++) 
            if (board.getCell(i, j) != board.getEmpty()) 
                for (int di = -1; di <= 1; di++) 
                    for (int dj = -1; dj <= 1; dj++) 
                    {
                        int ni = i + di;
                        int nj = j + dj;
                        
                        if (ni >= 0 && ni < size && nj >= 0 && nj < size &&
                            board.getCell(ni, nj) == board.getEmpty() &&
                            !considered[ni * size + nj]) 
                        {
                            moves.emplace_back(ni, nj);
                            considered[ni * size + nj] = true;
                        }
                    }

    if (moves.empty()) 
    {
        int center = size / 2;
        if (board.isCellEmpty(center, center))
            moves.emplace_back(center, center);
    }
    
    std::sort(moves.begin(), moves.end(),
        [&](const std::pair<int, int>& a, const std::pair<int, int>& b) 
        {
            int scoreA = quickMoveScore(board, a.first, a.second, 
                                       forAI ? aiMark : playerMark, forAI);
            int scoreB = quickMoveScore(board, b.first, b.second, 
                                       forAI ? aiMark : playerMark, forAI);
            return scoreA > scoreB;
        });
    
    return moves;
}

int MinimaxAI::minimax(Board& board, int depth, int alpha, int beta,
                      bool maximizingPlayer) 
{
    if (timeUp()) 
        return 0;
    
    if (depth == 0 || game->isTerminal(board))
        return game->evaluate(board);
   
    auto moves = getOrderedMoves(board, maximizingPlayer);    
   
    if (maximizingPlayer) 
    {
        int maxEval = EngineConst::NEG_INF;
        
        for (const auto& move : moves) 
        {
            if (timeUp()) break;
           
            setMove(board, move.first, move.second, aiMark);
           
            int eval = minimax(board, depth - 1, alpha, beta, false);
           
            undoMove(board, move.first, move.second);
           
            maxEval = std::max(maxEval, eval);
           
            alpha = std::max(alpha, eval);
            if (beta <= alpha)
                break;
        }
        
        return maxEval;
    } 
    else 
    {
        int minEval = EngineConst::INF;
        
        for (const auto& move : moves) 
        {
            if (timeUp()) break;
            
            setMove(board, move.first, move.second, playerMark);
            int eval = minimax(board, depth - 1, alpha, beta, true);
            undoMove(board, move.first, move.second);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            if (beta <= alpha)
                break;
        }
        
        return minEval;
    }
}

std::pair<int, int> MinimaxAI::iterativeDeepening(Board& board) 
{
    std::pair<int, int> bestMove = {-1, -1};
    int bestScore = EngineConst::NEG_INF;

    for (int depth = 1; depth <= max_depth; depth++) 
    {
        if (timeUp()) break;
        
        int currentBest = EngineConst::NEG_INF;
        std::pair<int, int> currentBestMove = {-1, -1};
       
        auto moves = getOrderedMoves(board, true);
        
        int alpha = EngineConst::NEG_INF;
        int beta = EngineConst::INF;
        
        for (const auto& move : moves) 
        {
            if (timeUp()) break;
           
            setMove(board, move.first, move.second, aiMark);
           
            int score = minimax(board, depth - 1, alpha, beta, false);
           
            undoMove(board, move.first, move.second);
           
            if (score > currentBest) 
            {
                currentBest = score;
                currentBestMove = move;
            }
            alpha = std::max(alpha, currentBest);
        }
       
        if (currentBestMove.first != -1 && !timeUp()) 
        {
            bestMove = currentBestMove;
            bestScore = currentBest;
           
            if (showThinking) 
            {
                std::string msg = "[AI] Depth " + std::to_string(depth) + 
                                 ": Move (" + std::to_string(bestMove.first) + 
                                 "," + std::to_string(bestMove.second) + 
                                 ") Score " + std::to_string(currentBest);
                thinkingLog.push_back(msg);
            }
           
            if (bestScore > EngineConst::WIN_SCORE / 2) 
            {
                if (showThinking)
                    thinkingLog.push_back("[AI] Find winning move, stop find");
                break;
            }
        }
    }
    return bestMove;
}

std::pair<int, int> MinimaxAI::findBestMove(Board& board) 
{   
    startTime = clock();
    thinkingLog.clear();
   
    auto emptyCells = board.getEmptyCells();
    for (const auto& cell : emptyCells) 
    {
        board.setCell(cell.first, cell.second, aiMark);
        if (board.checkWin(aiMark, win_length)) 
        {
            board.setCell(cell.first, cell.second, board.getEmpty());
            if (showThinking)
                thinkingLog.push_back("[AI] Find wininng move!");
            return cell;
        }
        board.setCell(cell.first, cell.second, board.getEmpty());
    }
    
    for (const auto& cell : emptyCells) 
    {
        board.setCell(cell.first, cell.second, playerMark);
        if (board.checkWin(playerMark, win_length)) 
        {
            board.setCell(cell.first, cell.second, board.getEmpty());
            if (showThinking)
                thinkingLog.push_back("[AI] Block player win!");
            return cell;
        }
        board.setCell(cell.first, cell.second, board.getEmpty());
    }
    
    std::pair<int, int> bestMove = iterativeDeepening(board);
   
    if (bestMove.first == -1 && !emptyCells.empty()) 
    {
        int bestScore = -1000000;
        for (const auto& cell : emptyCells) 
        {
            int score = quickMoveScore(board, cell.first, cell.second, aiMark, true);
            if (score > bestScore) 
            {
                bestScore = score;
                bestMove = cell;
            }
        }
        
        if (showThinking)
            thinkingLog.push_back("[AI] Using fallback strategy");
    }
    
    return bestMove;
}

bool MinimaxAI::isWinningMove(const Board& board, int row, int col, char mark) const 
{
    Board tempBoard = board;
    tempBoard.setCell(row, col, mark);
    return tempBoard.checkWin(mark, win_length);
}
