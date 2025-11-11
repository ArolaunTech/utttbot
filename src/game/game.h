#include <array>
#include <vector>
#include <string>

#ifndef GAME_H
#define GAME_H

class Game {
private:
	std::array<int, 81> cells;
	std::array<int, 9> megacells;
	int turn;
	int board;

	std::vector<int> moves;
public:
	Game();

	void setFromFen(std::string fen);

	bool validClick(int cellindex);

	bool isOver();
	int eval();

	int getTurn();

	void makeMove(int cellindex);
	void unmakeMove(int cellindex);
	void unmakeMove();

	std::string toString();
};

#endif