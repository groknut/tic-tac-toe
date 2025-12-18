#ifndef _MINIMAX_H_
#define _MINIMAX_H_

#include "cfig.h"
#include "board.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <ctime>
#include <algorithm>

namespace EngineConst
{
    const int WIN_SCORE = 1000000;
    const int LOSS_SCORE = -WIN_SCORE;
    
    const int INF = 1000000000;
    const int NEG_INF = -INF;
    
    const std::vector<int> LINE_SCORES = {
        0,      // 0 фишек
        1,      // 1 фишка  (малоценно)
        10,     // 2 фишки  (потенциал)
        100,    // 3 фишки  (угроза)
        1000,   // 4 фишки  (почти победа)
        10000   // 5 фишек  (победа)
    };
};

class MinimaxAI
{
private:
    // ============= ОСНОВНЫЕ ПАРАМЕТРЫ =============
    char playerMark;   // Метка игрока (противник)
    char aiMark;       // Метка ИИ
    int max_depth;     // Максимальная глубина поиска
    int time_limit;    // Лимит времени в миллисекундах
    int win_length;    // Длина выигрышной линии
    clock_t startTime; // Время начала поиска
    bool showThinking; // Показывать процесс мышления

    std::vector<std::string> thinkingLog;

    int size;          // размер доски
    char emptyCell;    // значение пустой клетки
        
    // ============= ОСНОВНЫЕ МЕТОДЫ МИНИМАКСА =============
    // Проверка окончания времени
    bool timeUp() const;
    
    // Проверка терминального состояния (победа/поражение/ничья)
    bool isTerminal(const Board& board) const;
    
    // Установка и отмена хода (для моделирования)
    void setMove(Board& board, int row, int col, char mark);
    void undoMove(Board& board, int row, int col);
    
    // Оценка позиции
    int evaluate(const Board& board) const;
    int evaluatePosition(const Board& board) const;
    int evaluateLine(const Board& board, int startRow, int startCol, 
                    int deltaRow, int deltaCol) const;
    
    // Основной алгоритм минимакса с альфа-бета отсечением
    int minimax(Board& board, int depth, int alpha, int beta, bool maximizingPlayer);
    
    // ============= ОПТИМИЗАЦИИ =============
    
    // 1. ИТЕРАТИВНОЕ УГЛУБЛЕНИЕ
    std::pair<int, int> iterativeDeepening(Board& board);
    
    // 2. УПОРЯДОЧИВАНИЕ ХОДОВ
    std::vector<std::pair<int, int>> getOrderedMoves(const Board& board, bool forAI) const;
    
    // Оценка хода (для сортировки)
    int quickMoveScore(const Board& board, int row, int col, char mark, bool forAI) const;
    
    // 3. ОЦЕНКА УГРОЗ
    bool isWinningMove(const Board& board, int row, int col, char mark) const;
    bool isThreateningMove(const Board& board, int row, int col, char mark, int& threatLevel) const;
    
public:
    // Конструкторы
    MinimaxAI() {};
    MinimaxAI(const Cfig& cfig);
    
    // Основной метод для поиска лучшего хода
    std::pair<int, int> findBestMove(Board& board);

    const std::vector<std::string>& getThinkingLog() const { return thinkingLog; }
    void clearThinkingLog() { thinkingLog.clear(); }
};

#endif
