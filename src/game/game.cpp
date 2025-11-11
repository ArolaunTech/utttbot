#include <iostream>

#include "game.h"

const bool wins[8][9] = {
	{ true,  true,  true, false, false, false, false, false, false},
	{false, false, false,  true,  true,  true, false, false, false},
	{false, false, false, false, false, false,  true,  true,  true},
		
	{ true, false, false,  true, false, false,  true, false, false},
	{false,  true, false, false,  true, false, false,  true, false},
	{false, false,  true, false, false,  true, false, false,  true},
		
	{ true, false, false, false,  true, false, false, false,  true},
	{false, false,  true, false,  true, false,  true, false, false}
};

Game::Game() {
	for (int i = 0; i < 81; i++) {
		cells[i] = 0; // Blank
	}
	for (int i = 0; i < 9; i++) {
		megacells[i] = 0; // Blank
	}

	turn = 1; // X goes first
	board = 9; // Free reign

	moves.clear(); // Stack so we can do Game::unmakeMove()
}

void Game::setFromFen(std::string fen) {
	if (fen.length() != 81) {
		std::cout << "Warning: invalid fen length\n";
		return;
	}

	for (int i = 0; i < 81; i++) {
		switch (fen[i]) {
			case ' ':
				cells[i] = 0;
				break;
			case 'x':
			case 'X':
				cells[i] = 1;
				break;
			case 'o':
			case 'O':
				cells[i] = -1;
				break;
			default:
				std::cout << "Warning: invalid char\n";
				return;
		}
	}

	for (int i = 0; i < 9; i++) {
		megacells[i] = 0;
		for (int player = -1; player <= 1; player += 2) {
			for (int poss = 0; poss < 8; poss++) {
				bool valid = true;

				for (int cell = 0; cell < 9; cell++) {
					if (!wins[poss][cell]) continue;

					if (cells[9 * i + cell] != player) {
						valid = false;
						break;
					}
				}

				if (valid) {
					megacells[i] = player;
					//std::cout << "Setting megacell " << i << " to " << player << " under poss " << poss << "\n";
				}
			}
		}
	}

	int lastplayed = -1;
	turn = 1;
	for (int i = 0; i < 81; i++) {
		switch (fen[i]) {
			case 'X':
				turn = -1;
				lastplayed = i;
				break;
			case 'O':
				turn = 1;
				lastplayed = i;
				break;
		}
	}

	//std::cout << lastplayed << " lastplay\n";

	board = 9;
	moves.clear();
	if (lastplayed != -1) {
		moves.push_back(lastplayed);

		board = lastplayed % 9;

		//std::cout << board << " board\n";
		//std::cout << megacells[board] << " megacell\n";

		bool empty = false;
		for (int cell = 0; cell < 9; cell++) {
			if (cells[9 * board + cell] == 0) {
				empty = true;
				break;
			}
		}

		if (!empty) board = 9;
		if (megacells[board] != 0) board = 9;
	}
}

bool Game::validClick(int cellindex) {
	int clickboardindex = cellindex / 9;

	if (clickboardindex != board && board != 9) return false; // Clicked on the wrong board
	if (megacells[clickboardindex] != 0) return false; // Clicked on a solved board
	if (cells[cellindex] != 0) return false; // Clicked on an occupied cell

	return true;
}

bool Game::isOver() {
	for (int player = -1; player <= 1; player += 2) {
		for (int win = 0; win < 8; win++) {
			bool valid = true;

			for (int cell = 0; cell < 9; cell++) {
				if (!wins[win][cell]) continue;
				if (megacells[cell] != player) {
					valid = false;
					break;
				}
			}

			if (valid) return true;
		}
	}

	for (int megacell = 0; megacell < 9; megacell++) {
		if (megacells[megacell] != 0) continue;

		for (int cell = 0; cell < 9; cell++) {
			if (cells[9 * megacell + cell] == 0) return false;
		}
	}

	return true;
}

int Game::eval() {
	for (int player = -1; player <= 1; player += 2) {
		for (int win = 0; win < 8; win++) {
			bool valid = true;

			for (int cell = 0; cell < 9; cell++) {
				if (!wins[win][cell]) continue;
				if (megacells[cell] != player) {
					valid = false;
					break;
				}
			}

			if (valid) return player;
		}
	}

	for (int megacell = 0; megacell < 9; megacell++) {
		if (megacells[megacell] != 0) continue;

		for (int cell = 0; cell < 9; cell++) {
			if (cells[9 * megacell + cell] == 0) return 2;
		}
	}

	return 0;
}

int Game::getTurn() {
	return turn;
}

void Game::makeMove(int cellindex) {
	int clickboardindex = cellindex / 9;
	int newboardindex = cellindex % 9;

	if (!validClick(cellindex)) return;

	moves.push_back(cellindex);

	cells[cellindex] = turn;

	for (int i = 0; i < 8; i++) {
		bool valid = true;

		for (int cell = 0; cell < 9; cell++) {
			if (!wins[i][cell]) continue;
			if (cells[9 * clickboardindex + cell] != turn) {
				valid = false;
				break;
			}
		}

		if (valid) megacells[clickboardindex] = turn;
	}

	turn *= -1;

	board = newboardindex;
	if (megacells[newboardindex] != 0) board = 9;

	bool empty = false;
	for (int cell = 0; cell < 9; cell++) {
		if (cells[9 * newboardindex + cell] == 0) {
			empty = true;
			break;
		}
	}

	if (!empty) board = 9;
}

void Game::unmakeMove(int cellindex) {
	// Unmaking a move on an empty game.
	// For non-buggy code this condition should never trigger.
	if (moves.empty()) return;

	// Unmaking the wrong move.
	// For non-buggy code this condition should never trigger.
	if (moves[moves.size() - 1] != cellindex) return;

	moves.pop_back();

	cells[cellindex] = 0;
	megacells[cellindex / 9] = 0;

	turn *= -1;

	board = 9;
	if (!moves.empty()) {
		int boardindex = moves[moves.size() - 1] % 9;

		if (megacells[boardindex] == 0) {
			bool empty = false;
			for (int cell = 0; cell < 9; cell++) {
				if (cells[9 * boardindex + cell] == 0) {
					empty = true;
					break;
				}
			}

			if (empty) board = boardindex;
		}
	}
}

void Game::unmakeMove() {
	// Unmaking a move on an empty game.
	// For non-buggy code this condition should never trigger.
	if (moves.empty()) return;

	int cellindex = moves[moves.size() - 1];

	moves.pop_back();

	cells[cellindex] = 0;
	megacells[cellindex / 9] = 0;

	turn *= -1;

	board = 9;
	if (!moves.empty()) {
		int boardindex = moves[moves.size() - 1] % 9;

		if (megacells[boardindex] == 0) {
			bool empty = false;
			for (int cell = 0; cell < 9; cell++) {
				if (cells[9 * boardindex + cell] == 0) {
					empty = true;
					break;
				}
			}

			if (empty) board = boardindex;
		}
	}
}

std::string Game::toString() {
	std::string out = "";

	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			int boardindex = 3 * (row / 3) + col / 3;
			int cellindex = 3 * (row % 3) + col % 3;

			int index = 9 * boardindex + cellindex;

			switch (cells[index]) {
			case 1:
				out += "X";
				break;
			case -1:
				out += "O";
				break;
			default:
				out += ".";
			}

			if (col == 2 || col == 5) {
				out += " | ";
			} else {
				out += " ";
			}
		}
		out += "\n";

		if (row == 2 || row == 5) {
			out += "------+-------+------\n";
		}
	}

	out += "\nMegacells: ";

	for (int i = 0; i < 9; i++) {
		switch (megacells[i]) {
		case 1:
			out += "X";
			break;
		case -1:
			out += "O";
			break;
		default:
			out += ".";
		}

		out += " ";
	}

	out += "\n";

	return out;
}