#include <array>

#ifndef GAME_H
#define GAME_H

class Game {
private:
	std::array<int, 81> cells;
	std::array<bool, 9> solved;
	int turn;
	int board;
public:
	Game();

	Game makeMove(int cellindex);
};

#endif