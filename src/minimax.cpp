#include "../head/minimax.h"

bool MinimaxAI::timeUp() const 
{
    return (clock() - startTime) * 1000 / CLOCKS_PER_SEC > time_limit;
}

MinimaxAI::MinimaxAI(char aiMark, char playerMark, int max_depth, int time_limit, int win_length)
    : aiMark(aiMark), playerMark(playerMark), max_depth(max_depth),
      time_limit(time_limit), win_length(win_length) {}


MinimaxAI::MinimaxAI(const Cfig& cfig)
{
	aiMark = cfig("player2", "mark").toChar();
	playerMark = cfig("player1", "mark").toChar();
	max_depth = cfig("AI", "depth").toInt();
	time_limit = cfig("AI", "time_limit").toInt();
	win_length = cfig("game", "win_length").toInt();
}

std::string MinimaxAI::serializeBoard(const Board& board) 
{
     std::string s;
     int size = board.getSize();
     s.reserve(size * size);
     for (int i = 0; i < size; i++) 
     {
         for (int j = 0; j < size; j++) 
         {
             char cell = board.getCell(i, j);
             s.push_back(cell == ' ' ? '.' : cell);
         }
     }
     return s;
}

bool MinimaxAI::isTerminal(Board& board) {
     return board.checkWin(aiMark, win_length) || 
            board.checkWin(playerMark, win_length) || 
            board.isFull();
}

int MinimaxAI::evaluateWindowAI(const int& pc, const int& openEnds)
{
	int score = 0;
	if (pc == 4 && openEnds >= 1) score = 50000;
	else if (pc == 3 && openEnds >= 1) score = 5000;
	else if (pc == 2 && openEnds >= 1) score = 500;
	else if (pc == 1 && openEnds >= 1) score = 10;
	
	score *= (openEnds == 2 ? 2 : 1);
	return score;
}

int MinimaxAI::evaluateWindowPlayer(const int& oc, const int& openEnds)
{
	int score = 0;
	if (oc == 4 && openEnds >= 1) score = -50000;
	else if (oc == 3 && openEnds >= 1) score = -5000;
	else if (oc == 2 && openEnds >= 1) score = -500;
	else if (oc == 1 && openEnds >= 1) score = -10;
	
	score *= (openEnds == 2 ? 2 : 1);
	return score;
}

int MinimaxAI::evaluateWindow(const int& pc, const int& oc, const int& openEnds) 
{

    if (pc > 0 && oc > 0) return 0;
    
    if (pc >= win_length) return EngineConst::WIN_SCORE;
    if (oc >= win_length) return EngineConst::LOSS_SCORE;
    
    if (pc > 0)
        return evaluateWindowAI(pc, openEnds);
    else if (oc > 0)
		return evaluateWindowPlayer(oc, openEnds);
    
    return 0;
}

int MinimaxAI::evaluateLines(const Board& board)
{
	int size = board.getSize(), score = 0;
	    
    // Оценка по всем направлениям
    const int dirs[4][2] = {
    	{0, 1}, {1, 0}, {1, 1}, {1, -1}
    };

	    
    for (int row = 0; row < size; row++) 
    {
        for (int col = 0; col < size; col++) 
        {
            for (int d = 0; d < 4; d++) 
            {
                int dr = dirs[d][0];
                int dc = dirs[d][1];
                
                // Проверяем окно длиной win_length
                if (row + dr * (win_length - 1) < size && 
                    col + dc * (win_length - 1) >= 0 && 
                    col + dc * (win_length - 1) < size) 
                    {
	                    int pc = 0, oc = 0, openEnds = 0;
	                    
	                    // Считаем символы в окне
	                    for (int k = 0; k < win_length; k++) 
	                    {
	                        char cell = board.getCell(row + dr * k, col + dc * k);
	                        if (cell == aiMark) 
	                        	pc++;
	                        else if (cell == playerMark) 
	                        	oc++;
	                    }	
	                    // Проверяем открытые концы
	                    if (row - dr >= 0 && col - dc >= 0 && col - dc < size) 
	                    {
	                        char before = board.getCell(row - dr, col - dc);
	                        if (before == board.getEmpty()) 
	                        	openEnds++;
	                    	else
		                        openEnds++;
	                    }
	                    
	                    if (row + dr * win_length < size && 
	                        col + dc * win_length >= 0 && 
	                        col + dc * win_length < size) {
	                        char after = board.getCell(row + dr * win_length, col + dc * win_length);
	                        if (after == board.getEmpty()) 
	                        	openEnds++;
	                    }
	                    score += evaluateWindow(pc, oc, openEnds);
                }
            }
        }
    }
    return score;
}

bool MinimaxAI::undoMove(Board& board, const int& row, const int& col)
{
    if (row < 0 || row >= board.getSize() || col < 0 || col >= board.getSize() )
        return false;
    if (board.getCell(row, col) == board.getEmpty())
        return false;
    board.setCell(row, col, board.getEmpty());
    return true;
}

int MinimaxAI::evaluateCenter(const Board& board)
{
	int size = board.getSize(), score = 0;
	
	int center = size / 2;
	for (int i = 0; i < size; i++) 
	{
	    for (int j = 0; j < size; j++) 
	    {
	        char cell = board.getCell(i, j);
	        if (cell == aiMark) 
	        {
	            int distFromCenter = abs(i - center) + abs(j - center);
	            score += (size - distFromCenter) * 5;
	        } 
	        else if (cell == playerMark) 
	        {
	            int distFromCenter = abs(i - center) + abs(j - center);
	            score -= (size - distFromCenter) * 5;
	        }
	    }
	}
	return score;
}

int MinimaxAI::heuristicEval(const Board& board) 
{
    if (board.checkWin(aiMark, win_length)) return EngineConst::WIN_SCORE;
    if (board.checkWin(playerMark, win_length)) return EngineConst::LOSS_SCORE;
    
    int score = 0;
    score += evaluateLines(board);    // Оценка линий и угроз
    score += evaluateCenter(board);   // Оценка контроля центра
    
    return score;
}

bool MinimaxAI::isBoardEmpty(const Board& board)
{
	return board.getEmptyCells().size() == board.getSize() * board.getSize();
}

std::pair<int, int> MinimaxAI::getBoardCenter(const Board& board)
{
    int center = board.getSize() / 2;
    return {center, center};
}

void MinimaxAI::addFreeNeighs(
    Board& board, int row, int col, std::vector<std::pair<int, int>>& moves, std::vector<std::vector<bool>>& seen
)
{
    int n = board.getSize();
    
    for (int dr = -2; dr <= 2; dr++)
        for (int dc = -2; dc <= 2; dc++) {
            
            int r = row + dr;
            int c = col + dc;
            
            if (r >= 0 && r < n && c >= 0 && c < n && board.isCellEmpty(r, c) && !seen[r][c]) {                
                moves.push_back({r, c});
                seen[r][c] = true;
            }
    }
}

std::vector<std::pair<int, int>> MinimaxAI::collectNeighborMoves(Board& board)
{
    std::vector<std::pair<int, int>> moves;
    int size = board.getSize();
    
    // Отслеживаем уже добавленные клетки
    std::vector<std::vector<bool>> marked(size, std::vector<bool>(size, false));
    
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            if (board.getCell(row, col) != board.getEmpty()) {
                addFreeNeighs(board, row, col, moves, marked);
            }
        }
    }
    
    return moves;
}

std::vector<std::pair<int, int>> MinimaxAI::generateCandidateMoves(Board& board)
{
	std::vector<std::pair<int, int>> moves;
    int size = board.getSize();

    if (isBoardEmpty(board))
    {
    	moves.push_back(getBoardCenter(board));
    	return moves;
    }

	moves = collectNeighborMoves(board);
	
	if (moves.empty())
        return board.getEmptyCells();
    
    return moves;
    
}

bool MinimaxAI::findKillMove(Board& board, const char& mark, std::pair<int, int>& move)
{
    for (const auto& cell : board.getEmptyCells()) 
    {
        board.setCell(cell.first, cell.second, mark);
        
        bool isWinning = board.checkWin(mark, win_length);
        undoMove(board, cell.first, cell.second);
        
        if (isWinning)
        {
            move = cell;
            return true;
        }
    }
    
    return false;
}

bool MinimaxAI::scanKillMoves(Board& board, std::pair<int, int>& move) 
{

	if (findKillMove(board, aiMark, move))
		return true;

	if (findKillMove(board, playerMark, move))
		return true;

	return false;
}

int MinimaxAI::rateMove(Board& board, const std::pair<int, int>& move)
{
    int score = getCenterScore(board, move) + getNeighScore(board, move);
    return score;
}

int MinimaxAI::getNeighScore(Board& board, const std::pair<int, int>& move)
{
    int score = 0;
    int size = board.getSize();
    
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            
            int r = move.first + dr;
            int c = move.second + dc;
            
            if (r >= 0 && r < size && c >= 0 && c < size) {
                char cell = board.getCell(r, c);
                if (cell == aiMark) score += 50;
                else if (cell == playerMark) score += 30;
            }
        }
    }
    
    return score;
}

int MinimaxAI::getCenterScore(Board& board, const std::pair<int, int>& move)
{
    int size = board.getSize();
    int center = size / 2;
    int dist = abs(move.first - center) + abs(move.second - center);
    return (size - dist) * 10;
}

void MinimaxAI::orderMoves(Board& board, std::vector<std::pair<int, int>>& moves) 
{
    if (moves.empty()) 
    	return;
    
    std::vector<std::pair<int, std::pair<int, int>>> rated;

    rated.reserve(moves.size());

    for (const auto& move : moves)
    {
    	int score = rateMove(board, move);
    	rated.push_back({score, move});
    }

    std::sort(
    	rated.begin(), rated.end(), [](auto& a, auto& b) { return a.first > b.first; }
    );

    moves.clear();
    for (auto& item : rated)
    	moves.push_back(item.second);
}

int MinimaxAI::Maximizing(
    Board& board, int depth, int& alpha, int& beta, const std::vector<std::pair<int, int>>& moves
)
{
	int bestScore = EngineConst::NEG_INF;
	
	for (const auto& move : moves) 
	{
	    board.setCell(move.first, move.second, aiMark);
	    
	    int score = minimax(board, depth - 1, alpha, beta, false);
	    
	    undoMove(board, move.first, move.second);
	    
	    if (timeUp()) 
	    	return bestScore;
	    
	    bestScore = std::max(bestScore, score);
	    alpha = std::max(alpha, bestScore);
	    
	    if (alpha >= beta) 
	    	break;
   }
   return bestScore;
}

int MinimaxAI::Minimizing(
    Board& board, int depth, int& alpha, int& beta, const std::vector<std::pair<int, int>>& moves
)
{
	int bestScore = EngineConst::INF;
	        
    for (const auto& move : moves) {
        // Делаем ход
        board.setCell(move.first, move.second, playerMark);
        
        // Рекурсивный вызов
        int score = minimax(board, depth - 1, alpha, beta, true);
        
        // Отменяем ход
        undoMove(board, move.first, move.second);
        
        // Проверка времени
        if (timeUp()) return bestScore;
        
        // Обновление лучшего счета
        bestScore = std::min(bestScore, score);
        beta = std::min(beta, bestScore);
        
        // Альфа-бета отсечение
        if (alpha >= beta) break;
    }
    
    return bestScore;
}

int MinimaxAI::minimax(Board& board, int depth, int alpha, int beta, bool maximizing) 
{
    if (timeUp()) 
    	return 0;
    
    if (depth == 0 || isTerminal(board)) {
        return heuristicEval(board);
    }
    
    std::string boardKey = serializeBoard(board);
    auto it = tt.find(boardKey);
    if (it != tt.end()) {
        return it->second;
    }
    
    auto moves = generateCandidateMoves(board);
    if (moves.empty()) {
        int score = heuristicEval(board);
        tt[boardKey] = score;
        return score;
    }
    
    orderMoves(board, moves);
    
    if (maximizing)
        bestScore = Maximizing(board, depth, alpha, beta, moves);
    else
    	bestScore = Minimizing(board, depth, alpha, beta, moves);        

    tt[boardKey] = bestScore;
    return bestScore;
}

std::pair<int, int> MinimaxAI::findBestMove(const Board& board) 
{
    startTime = clock();
    
    std::pair<int, int> bestMove = {-1, -1};
    
    Board boardCopy = board;
    
    std::pair<int, int> kill_move;
    if (scanKillMoves(boardCopy, kill_move))
        return kill_move;
    
    auto moves = generateCandidateMoves(boardCopy);
    if (moves.empty()) return {-1, -1};
    
    orderMoves(boardCopy, moves);
    
    int bestScore = EngineConst::NEG_INF;

    for (int currentDepth = 1; currentDepth <= max_depth; currentDepth++) 
    {
        if (timeUp()) 
        	break;
        
        int currentBestScore = EngineConst::NEG_INF;
        std::pair<int, int> currentBestMove = moves[0];
        
        for (const auto& move : moves) 
        {
            if (timeUp()) 
            	break;
            
            boardCopy.setCell(move.first, move.second, aiMark);
            
            int score = minimax(
            	boardCopy, currentDepth - 1, EngineConst::NEG_INF, EngineConst::INF, false
            );
            
            undoMove(boardCopy, move.first, move.second);
            
            if (score > currentBestScore) 
            {
                currentBestScore = score;
                currentBestMove = move;
            }
        }
        
        if (currentBestScore > bestScore) 
        {
            bestScore = currentBestScore;
            bestMove = currentBestMove;
        }
        
        if (bestScore >= EngineConst::WIN_SCORE - 1000)
            break;
    }
    
    if (bestMove.first == -1) 
    {
        auto emptyCells = board.getEmptyCells();
        if (!emptyCells.empty())
            bestMove = emptyCells[0];
    }
    
    return bestMove;
}
