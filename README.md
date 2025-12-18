
## tic-tac-toe

### Описание работы алгоритма

1. Текущее состояние игры передается в функцию оценки
2. Результат функии оценки передается в алгоритм Минимакс
3. Если `beta` <= `alpha` отсекаем ветвь решений.

### Начало работы
1. Склонируйте проект
```bash
git clone https://github.com/groknut/tic-tac-toe.git
cd tic-tac-toe
```
2. Соберите проект
```bash
g++ -Ilibs/cfig main.cpp src/*.cpp libs/cfig/cfig.cpp -o main
```


### Пример конфигурации проекта
```ini
# параметры доски
[board]
size = 20
empty = '-'
# параметры игры
[game]
win_length = 5
start = 'X'
color = true
# параметры игроков
[player1]
type = "ai"
name = "Groknut"
mark = 'X'

[player2]
type = "ai"
name = "AI"
mark = "O"
# параметры ИИ
[AI]
depth = 6
time_limit = 10000
show_thinking = true
# для отладки
[debug]
sleep = 3000
clear_console = true
```
