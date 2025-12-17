#include "../head/minimax.h"


MinimaxAI::MinimaxAI(const Cfig& cfig) 
{   
    aiMark = cfig("player2", "mark").toChar();     
    playerMark = cfig("player1", "mark").toChar(); 
    max_depth = cfig("AI", "depth").toInt();       
    time_limit = cfig("AI", "time_limit").toInt(); 
    win_length = cfig("game", "win_length").toInt();
    showThinking = cfig.get<bool>("AI", "show_thinking", false);
    size = cfig("board", "size").toInt();
    emptyCell = cfig("board", "empty").toChar();
}

bool MinimaxAI::timeUp() const 
{   
    return (clock() - startTime) * 1000.0 / CLOCKS_PER_SEC > time_limit;
}


bool MinimaxAI::isTerminal(const Board& board) const 
{
    return board.checkWin(aiMark, win_length) ||
           board.checkWin(playerMark, win_length) ||
           board.isFull();
}

void MinimaxAI::setMove(Board& board, int row, int col, char mark) 
{
    board.setCell(row, col, mark);
}

void MinimaxAI::undoMove(Board& board, int row, int col) 
{
    board.setCell(row, col, emptyCell);
}


int MinimaxAI::quickMoveScore(const Board& board, int row, int col, char mark, bool forAI) const 
{
    int score = 0;
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
    std::vector<std::vector<bool>> considered(size, std::vector<bool>(size, false));

    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j < size; j++) 
        {
           
            if (board.getCell(i, j) != emptyCell) 
            {
                for (int di = -1; di <= 1; di++) 
                {
                    for (int dj = -1; dj <= 1; dj++) 
                    {
                        int ni = i + di;
                        int nj = j + dj;
                        
                        if (ni >= 0 && ni < size && nj >= 0 && nj < size &&
                            board.getCell(ni, nj) == emptyCell &&
                            !considered[ni][nj]) 
                        {
                            moves.emplace_back(ni, nj);
                            considered[ni][nj] = true;
                        }
                    }
                }
            }
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


int MinimaxAI::evaluateLine(const Board& board, int startRow, int startCol,
                           int deltaRow, int deltaCol) const 
{    
    int aiCount = 0, playerCount = 0, emptyCount = 0;
   
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
            return baseScore * (emptyCount + 1);
        return baseScore;
    }
    
   
    if (playerCount > 0 && aiCount == 0) {
        int baseScore = EngineConst::LINE_SCORES[playerCount];
        
        if (emptyCount > 0)
            return -baseScore * (emptyCount + 2);
        return -baseScore;
    }
    
    return 0;
}


int MinimaxAI::evaluatePosition(const Board& board) const 
{
    int score = 0;
   
    for (int row = 0; row < size; row++)
        for (int col = 0; col <= size - win_length; col++)
            score += evaluateLine(board, row, col, 0, 1);
    
   
    for (int col = 0; col < size; col++) {
        for (int row = 0; row <= size - win_length; row++) {
            score += evaluateLine(board, row, col, 1, 0);
        }
    }
    
   
    for (int row = 0; row <= size - win_length; row++) {
        for (int col = 0; col <= size - win_length; col++) {
            score += evaluateLine(board, row, col, 1, 1);
        }
    }
    
   
    for (int row = 0; row <= size - win_length; row++) {
        for (int col = win_length - 1; col < size; col++) {
            score += evaluateLine(board, row, col, 1, -1);
        }
    }
    
   
    int center = size / 2;
    for (int i = center - 1; i <= center + 1; i++) {
        for (int j = center - 1; j <= center + 1; j++) {
            if (i >= 0 && i < size && j >= 0 && j < size) {
                if (board.getCell(i, j) == aiMark)
                    score += 5;
                else if (board.getCell(i, j) == playerMark)
                    score -= 5;
            }
        }
    }
    
    return score;
}


int MinimaxAI::evaluate(const Board& board) const {
   
    if (board.checkWin(aiMark, win_length))
        return EngineConst::WIN_SCORE;
    else if (board.checkWin(playerMark, win_length))
        return EngineConst::LOSS_SCORE;
    
   
    return evaluatePosition(board);
}


int MinimaxAI::minimax(Board& board, int depth, int alpha, int beta,
                      bool maximizingPlayer) {
    
    if (timeUp()) 
        return 0;
    
    if (depth == 0 || isTerminal(board)) {
       
        return evaluate(board);
    }
    
   
    auto moves = getOrderedMoves(board, maximizingPlayer);
    
   
    if (maximizingPlayer) {
       
        int maxEval = EngineConst::NEG_INF;
        
        for (const auto& move : moves) {
            if (timeUp()) break;
            
           
            setMove(board, move.first, move.second, aiMark);
            
           
            int eval = minimax(board, depth - 1, alpha, beta, false);
            
           
            undoMove(board, move.first, move.second);
            
           
            maxEval = std::max(maxEval, eval);
            
           
            alpha = std::max(alpha, eval);
            if (beta <= alpha) {
               
               
               
               
               
                break;
            }
        }
        
        return maxEval;
    } else {
       
        int minEval = EngineConst::INF;
        
        for (const auto& move : moves) {
            if (timeUp()) break;
            
            setMove(board, move.first, move.second, playerMark);
            int eval = minimax(board, depth - 1, alpha, beta, true);
            undoMove(board, move.first, move.second);
            
            minEval = std::min(minEval, eval);
            beta = std::min(beta, eval);
            
            if (beta <= alpha) {
               
               
               
               
                break;
            }
        }
        
        return minEval;
    }
}


std::pair<int, int> MinimaxAI::iterativeDeepening(Board& board) {
    std::pair<int, int> bestMove = {-1, -1};
    int bestScore = EngineConst::NEG_INF;
    
   
    for (int depth = 1; depth <= max_depth; depth++) {
        if (timeUp()) break;
        
        int currentBest = EngineConst::NEG_INF;
        std::pair<int, int> currentBestMove = {-1, -1};
        
       
        auto moves = getOrderedMoves(board, true);
        
        int alpha = EngineConst::NEG_INF;
        int beta = EngineConst::INF;
        
        for (const auto& move : moves) {
            if (timeUp()) break;
            
           
            setMove(board, move.first, move.second, aiMark);
            
           
            int score = minimax(board, depth - 1, alpha, beta, false);
            
           
            undoMove(board, move.first, move.second);
            
           
            if (score > currentBest) {
                currentBest = score;
                currentBestMove = move;
                
               
                alpha = std::max(alpha, currentBest);
            }
        }
        
       
        if (currentBestMove.first != -1 && !timeUp()) {
            bestMove = currentBestMove;
            bestScore = currentBest;
            
           
            if (showThinking) {
                std::cout << "[AI] Глубина " << depth << ": ход ("
                         << bestMove.first << "," << bestMove.second
                         << ") оценка " << bestScore << std::endl;
            }
            
           
            if (bestScore > EngineConst::WIN_SCORE / 2) {
                if (showThinking) {
                    std::cout << "[AI] Найден выигрышный ход, прекращаю поиск" << std::endl;
                }
                break;
            }
        }
    }
    return bestMove;
}


std::pair<int, int> MinimaxAI::findBestMove(Board& board) {
   
    startTime = clock();
    
   
    
   
    auto emptyCells = board.getEmptyCells();
    for (const auto& cell : emptyCells) {
        board.setCell(cell.first, cell.second, aiMark);
        if (board.checkWin(aiMark, win_length)) {
            board.setCell(cell.first, cell.second, emptyCell);
            if (showThinking) {
                std::cout << "[AI] Нашел выигрышный ход!" << std::endl;
            }
            return cell;
        }
        board.setCell(cell.first, cell.second, emptyCell);
    }
    
   
    for (const auto& cell : emptyCells) {
        board.setCell(cell.first, cell.second, playerMark);
        if (board.checkWin(playerMark, win_length)) {
            board.setCell(cell.first, cell.second, emptyCell);
            if (showThinking) {
                std::cout << "[AI] Блокирую выигрыш противника!" << std::endl;
            }
            return cell;
        }
        board.setCell(cell.first, cell.second, emptyCell);
    }
    
   
    
    std::pair<int, int> bestMove = iterativeDeepening(board);
    
   
    
    if (bestMove.first == -1 && !emptyCells.empty()) {
       
        int bestScore = -1000000;
        for (const auto& cell : emptyCells) {
            int score = quickMoveScore(board, cell.first, cell.second, aiMark, true);
            if (score > bestScore) {
                bestScore = score;
                bestMove = cell;
            }
        }
        
        if (showThinking) {
            std::cout << "[AI] Использую fallback стратегию" << std::endl;
        }
    }
    
    return bestMove;
}

bool MinimaxAI::isWinningMove(const Board& board, int row, int col, char mark) const {
    Board tempBoard = board;
    tempBoard.setCell(row, col, mark);
    return tempBoard.checkWin(mark, win_length);
}
