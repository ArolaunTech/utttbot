#include <cmath>
#include <iostream>

#include "../game/game.h"
#include "solver.h"
#include "tree.h"
#include "../random/random.h"

int monteCarloRun(Game game) {
	Game curr = game;

	while (!curr.isOver()) {
		//Play random move
		std::vector<int> moves;

		for (int i = 0; i < 81; i++) {
			if (curr.validClick(i)) {
				moves.push_back(i);
			}
		}

		curr.makeMove(moves[randint(0, moves.size() - 1)]);
	}

	return curr.eval();
}

int Solver::getBestMove(Game game, int maxnodes, std::chrono::milliseconds duration) {
	/*========================= AI description =========================*
	 * - Use some kind of MCTS since minimax is apparently ineffective
	 *   due to the lack of a good evaluation function.
	 */

	MCTSNode root(0, 0);
	std::vector<MCTSNode*> path;

	Game curr = game;

	auto start = std::chrono::steady_clock::now();
	std::chrono::milliseconds millisecond(1); //Buffer

	for (int sim = 0; sim < maxnodes; sim++) {
		path.clear();

		path.push_back(&root);

		while (!(path[path.size() - 1])->children.empty()) {
			int bestindex = 0;
			double bestscore = 0;

			std::size_t numchildren = (path[path.size() - 1])->children.size();

			for (int i = 0; i < numchildren; i++) {
				double uctscore = 100;

				if (((path[path.size() - 1])->children[i]->runs) > 0) {
					uctscore = 
						(double)((path[path.size() - 1])->children[i]->wins) / 
						(path[path.size() - 1])->children[i]->runs + 
						0.4 * std::sqrt(
							std::log((path[path.size() - 1])->runs) / 
							((path[path.size() - 1])->children[i]->runs)
						);
				}

				//std::cout << 
				//	((path[path.size() - 1])->children[i]->runs) << " " << 
				//	((path[path.size() - 1])->children[i]->wins) << " " << 
				//	((path[path.size() - 1])->runs) << " " <<
				//	uctscore << " score\n";

				if (uctscore > bestscore) {
					bestscore = uctscore;
					bestindex = i;
				}
			}

			curr.makeMove((path[path.size() - 1])->actions[bestindex]);
			path.push_back((path[path.size() - 1])->children[bestindex].get());
		}

		//std::cout << path.size() << " path\n";

		int evaluation = curr.eval();
		if (evaluation == 2) {
			//Develop leaf node
			for (int i = 0; i < 81; i++) {
				if (!curr.validClick(i)) continue;

				(path[path.size() - 1])->actions.push_back(i);
				(path[path.size() - 1])->children.push_back(std::make_unique<MCTSNode>(0, 0));

				//std::cout << "action " << i << "\n";
			}

			curr.makeMove((path[path.size() - 1])->actions[0]);
			path.push_back((path[path.size() - 1])->children[0].get());

			//Monte Carlo simulation
			evaluation = monteCarloRun(curr);
		}

		//std::cout << evaluation << "\n";

		//Propagate upwards
		for (int i = path.size(); i > 0; i--) {
			(path[path.size() - 1])->runs++;

			if (curr.getTurn() == evaluation) (path[path.size() - 1])->wins++;
			if (evaluation == 0) {
				(path[path.size() - 1])->wins++;
				(path[path.size() - 1])->runs++;
			}

			curr.unmakeMove();
			path.pop_back();
		}

		if ((sim % 25 == 0) && (std::chrono::steady_clock::now() >= start + duration - millisecond)) break;
	}

	int bestindex = 0;
	int bestruns = 0;

	std::size_t numchildren = root.children.size();

	for (int i = 0; i < numchildren; i++) {
		int runs = root.children[i]->runs;

		//std::cout << runs << " ";

		if (runs > bestruns) {
			bestruns = runs;
			bestindex = i;
		}
	}
	//std::cout << "\n";

	return root.actions[bestindex];
}