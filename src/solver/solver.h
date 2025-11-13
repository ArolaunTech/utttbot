#include <chrono>

#ifndef SOLVER_H
#define SOLVER_H

class Solver {
public:
	int getBestMove(Game game, int maxnodes, std::chrono::milliseconds duration);
};

#endif