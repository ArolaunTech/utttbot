#include "game.h"

Game::Game() {
	for (int i = 0; i < 81; i++) {
		cells[i] = 0;
	}

	turn = 1; //X plays first
	board = 9;
}

Game Game::makeMove(int cellindex) {
	const bool wins[8][9] = {
		//Rows
		{ true,  true,  true, false, false, false, false, false, false},
		{false, false, false,  true,  true,  true, false, false, false},
		{false, false, false, false, false, false,  true,  true,  true},
		//Cols
		{ true, false, false,  true, false, false,  true, false, false},
		{false,  true, false, false,  true, false, false,  true, false},
		{false, false,  true, false, false,  true, false, false,  true},
		//Diagonals
		{ true, false, false, false,  true, false, false, false, true},
		{false, false,  true, false,  true, false,  true, false, false}
	};

	int boardindex = cellindex / 9;
	int newboardindex = cellindex % 9;

	if (solved[boardindex]) return *this;
	if (boardindex != board && board != 9) return *this;
	if (cells[cellindex] != 0) return *this;

	Game out = *this;

	out.turn *= -1;
	out.board = newboardindex;

	if (solved[newboardindex]) out.board = 9;

	out.cells[cellindex] = turn;

	for (int cell = -1; cell <= 1; cell += 2) {
		for (int win = 0; win < 8; win++) {
			bool won = true;
			for (int boardidx = 0; boardidx < 9; boardidx++) {
				if (!wins[win][boardidx]) continue;
				if (out.cells[9 * boardindex + boardidx] != cell) {
					won = false;
					break;
				}
			}

			if (won) {
				out.solved[boardindex] = true;
				break;
			}
		}
	}

	return out;
}