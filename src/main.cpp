#include <iostream>
#include <string>
#include <vector>

#include "game/game.h"
#include "solver/solver.h"
#include "tests/tests.h"

int main() {
	Game game;
    Solver solver;

	//runperfttests(game);

    while (!game.isOver()) {
        std::cout << game.toString();
        std::cout << "\n";

        int move = solver.getBestMove(game);

        std::cout << "Solver's move: " << move << "\n";

        game.makeMove(move);
    }

    std::cout << game.toString();

    int evaluation = game.eval();

    switch (evaluation) {
    case 1:
        std::cout << "X wins!\n";
        break;
    case -1:
        std::cout << "O wins!\n";
        break;
    default:
        std::cout << "Draw\n";
    }
}