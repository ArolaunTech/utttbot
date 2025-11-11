#include <memory>
#include <vector>

#ifndef TREE_H
#define TREE_H

struct MCTSNode {
	int runs;
	int wins;

	std::vector<std::unique_ptr<MCTSNode> > children;
	std::vector<int> actions;

	MCTSNode(int newruns, int newwins): runs(newruns), wins(newwins) {}
};

#endif