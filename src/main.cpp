#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "game/game.h"
#include "solver/solver.h"
#include "tests/tests.h"

int main() {
    //Game game;

    //int nodescount = 0;

    //perft(game, 3, nodescount);

    //runperfttests(game);

    // UGI parsing + engine commanding

    const int NODE_LIMIT = 10000000;
    const std::chrono::milliseconds TIME_LIMIT(300000); // 5 minutes

    Game game;
    Solver solver;
    std::string command;

    while (true) {
        std::getline(std::cin, command);

        std::istringstream iss(command);
        std::string word;
        std::vector<std::string> words;

        while (iss >> word) {
            words.push_back(word);
        }

        /* 
        Unsupported commands:
         - debug [on / off]
         - setoption
         - register
         - go depth (doesn't make sense for an MCTS-based bot)
         - go infinite
         - stop (I don't want to deal with multithreading right now)
        */

        if (words.size() == 0) continue;

        if (words[0] == "ugi") {
            std::cout << "id name utttbot\n";
            std::cout << "id author ArolaunTech\n";
            std::cout << "ugiok\n";
        } else if (words[0] == "isready") {
            std::cout << "readyok\n";
        } else if (words[0] == "uginewgame") {

        } else if (words[0] == "position") {
            if (words.size() < 2) continue;

            if (words[1] == "startpos") {
                game.reset();

                for (int i = 3; i < words.size(); i++) {
                    std::string move = words[i];

                    if (move.length() != 2) continue;
                    if (move[0] < 'a' || move[0] > 'i') continue;
                    if (move[1] < '0' || move[1] > '9') continue;

                    int row = move[1] - '0'; //sort of like chess i guess
                    int col = move[0] - 'a';

                    int board = 3 * (row / 3) + col / 3;
                    int cell = 3 * (row % 3) + col % 3;

                    int index = 9 * board + cell;

                    game.makeMove(index);
                }
            } else if (words[1] == "fen") {
                game.setFromFen(words[2]);

                for (int i = 4; i < words.size(); i++) {
                    std::string move = words[i];

                    if (move.length() != 2) continue;
                    if (move[0] < 'a' || move[0] > 'i') continue;
                    if (move[1] < '0' || move[1] > '9') continue;

                    int row = move[1] - '0'; //sort of like chess i guess
                    int col = move[0] - 'a';

                    int board = 3 * (row / 3) + col / 3;
                    int cell = 3 * (row % 3) + col % 3;

                    int index = 9 * board + cell;

                    game.makeMove(index);
                }
            }
        } else if (words[0] == "query") {
            if (words.size() != 2) continue;

            if (words[1] == "gameover") {
                std::cout << "response " << (game.isOver() ? "true" : "false") << "\n";
            } else if (words[1] == "p1turn") {
                std::cout << "response " << ((game.getTurn() == 1) ? "true" : "false") << "\n";
            } else if (words[1] == "result") {
                int evaluation = game.eval();

                switch (evaluation) {
                case 1:
                    std::cout << "response p1win\n";
                    break;
                case -1:
                    std::cout << "response p2win\n";
                    break;
                case 0:
                    std::cout << "response draw\n";
                    break;
                case 2:
                    std::cout << "response none\n";
                    break;
                }
            }
        } else if (words[0] == "go") {
            if (words.size() < 3) continue;

            int bestmove = -1;

            if (words[1] == "movetime") {
                bestmove = solver.getBestMove(
                    game, 
                    NODE_LIMIT, 
                    std::chrono::milliseconds {std::stoi(words[2])}
                );
            } else if (words[1] == "nodes") {
                bestmove = solver.getBestMove(
                    game,
                    std::stoi(words[2]),
                    TIME_LIMIT
                );
            } else {
                int time = 0;
                int increment = 0;

                for (int i = 1; i < words.size() - 1; i++) {
                    //std::cout << words[i] << "\n";

                    if ((words[i] == "p1time") && (game.getTurn() == 1)) {
                        time = std::stoi(words[i + 1]);
                    } else if ((words[i] == "p1inc") && (game.getTurn() == 1)) {
                        increment = std::stoi(words[i + 1]);
                    } else if ((words[i] == "p2time") && (game.getTurn() == -1)) {
                        time = std::stoi(words[i + 1]);
                    } else if ((words[i] == "p2inc") && (game.getTurn() == -1)) {
                        increment = std::stoi(words[i + 1]);
                    }
                }

                int budget = increment + time / game.getEmptySpaces(); //Dumb heuristic

                //std::cout << "info " << budget << " " << increment << " " << time << " " << game.getEmptySpaces() << "\n";

                bestmove = solver.getBestMove(
                    game,
                    NODE_LIMIT,
                    std::chrono::milliseconds {budget} //illeh
                );
            }

            if (bestmove != -1) {
                int board = bestmove / 9;
                int cell = bestmove % 9;

                int row = 3 * (board / 3) + cell / 3;
                int col = 3 * (board % 3) + cell % 3;

                std::cout << "bestmove " << static_cast<char>(col + 'a') << static_cast<char>(row + '0') << "\n";
            }
        } else if (words[0] == "quit") return 0;
    }
}