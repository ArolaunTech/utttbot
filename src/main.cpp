#include <iostream>
#include <string>
#include <vector>

#include "game/game.h"

int perft(Game& game, int depth) {
	if (depth == 0) return 1;
	if (game.isOver()) return 0;

	int out = 0;
	for (int i = 0; i < 81; i++) {
		if (!game.validClick(i)) continue;

		game.makeMove(i);
		out += perft(game, depth - 1);
		game.unmakeMove(i);
	}

	return out;
}

void runperfttests(Game& game) {
	std::vector<std::string> fens = {
		"                                                                                 ",
        "ox  x      o  x o  x     x     o    x o    x        O          oo x              ",
        "xo  o  xox x       o  o    o        x  x                              Oo  x    x ",
        " ox     x    x x  x     o x          o      o         o      x o          oOx    ",
        " x xx          o  o        o      x o    o  o    x           x   O     o  x x    ",
        "o    x xx x   O oo  oo x    x  x   o  x oxo oo  oo   o x       o  xx    x x xx o ",
        "o    oox xox    o  xo  oox   x        O      xxo  oxxoxox  x      o xo o    xx   ",
        "xxO o oxx x  o  o      o   o    x   o     xx x   x  o x    oo x o x  xoxo xo    o",
        "  o xox x      xo   xo   o x x  O oxo  o    o x  x    o  o    ox  xx x  oo x o  x",
        "   ooxx     o xx  x        o    x xx x   ox x  o oxOo o  x  oooxo    x   o ox    ",
        " o    o ox    x  x      oo xoxxxx    o   o x    o x oo   o  xOxx  ox      xoo x  ",
        "xxox    ooooo    o  oxxx x o oxxxo   xo  o xO xxoooxo   xo x o xx   ox oox xox  x",
        "oo o xxo      oooo       ox ox o  o x   ox oxxx xxoooo x xxxo  xx xox xxxx xOoo o",
        "   x  o x   xxox o oxxxx    oo  xoo o  x oo   oo oxo ooxo xx xxx o  xxoxO xo xxoo",
        " x  ox x    ox x oooxo    oxo xxx ox  ooxooxx  xoo   o   xo x oo x  O oxoxxoxx xo",
        "xx oxx x o o  xx xoo   ooxx x  oox    xxoxo    x oxxoxooxx o xoO o  ox xo  o  oo ",
        "  o   xxxoox oxoxxxx o ox oxo oxoxoxo   ooxxo xx xox ooxoxxoooxxxoxOxo oooxxxooox",
        "x ox ox  ooxxoxoo  oxox xo  xoooxxxooxxxoxx xoooox   oxxoooxxoo  xxoxoxoxxo o Oxx",
        "xx xooOxoooxooxxxoxo o xo o ox x x   xooxoxx ox xxoxoxxoxoox ooxx o oo xoxx xooox",
        "xx oOxooxx  xooooooxo xxox oxooxoxoxxoxooooo oxoxx o x xxxx oxxxxoxxo  xo   ox  o",
        "o   o xxxooooxxxo oxxxoxooxooxx ooxxxxxxoo   xoooOxxooo  xo  ooxxx x x oxxoo oxox",
        "xoo x oxooxoxoxxxoxo o xxxooxxx  xooOo o    xxxoo o  x xxo ooxxoox xxooxxooxxooox",
        "oxoxoxo  xo x oOoo xxoo oxoxxx xxxooxxoooxx xxooo xxooo  ooox xo xoxo xoxxxo   xx",
        "  oo xoox  xx oxxoxoo  oxooooxxoo xxx xo xoxooxxoxxx ox xx ooooxoOxxooxo x oxoxx ",
        "ox  oxx    o  xo o o x  o xox  x  o x   xooooox ooxx ox oxx oox  x  x  xo xoXoxox"
	};
	std::vector<std::vector<int> > results = {
		{1, 81, 720, 6336, 55080, 473256, 4020960, 33782544, 281067408},
        {1, 6,  44,  320,  2278,  16233,  116635,  849418,   6381392  },
        {1, 7,  51,  370,  2827,  21383,  179029,  1487259,  13593237 },
        {1, 9,  64,  454,  3185,  23060,  166468,  1260336,  9736622  },
        {1, 8,  58,  463,  3479,  29053,  241143,  2173280,  19748086 },
        {1, 44, 391, 3436, 31662, 289755, 2792347, 26647358, 264767468},
        {1, 4,  28,  239,  2212,  21384,  196693,  1923003,  18155997 },
        {1, 8,  86,  694,  5205,  40777,  319881,  2664061,  22872400 },
        {1, 7,  67,  840,  9609,  115330, 1283277, 14818322, 158683651},
        {1, 41, 440, 4759, 48816, 496752, 4825482, 47240207, 442983131},
        {1, 6,  33,  298,  2978,  27462,  251373,  2277374,  20505230 },
        {1, 3,  22,  170,  1292,  7611,   42488,   178604,   683640   },
        {1, 4,  58,  519,  4456,  33205,  232391,  1384237,  7568559  },
        {1, 6,  63,  414,  2614,  17476,  108288,  680618,   3769073  },
        {1, 5,  23,  171,  1094,  7508,   47807,   322940,   2032799  },
        {1, 22, 163, 1457, 10431, 82349,  519427,  3451682,  17775153 },
        {1, 3,  4,   1,    0,     0,      0,       0,        0        },
        {1, 3,  6,   5,    0,     0,      0,       0,        0        },
        {1, 9,  35,  123,  327,   695,    1090,    1359,     896      },
        {1, 3,  4,   2,    0,     0,      0,       0,        0        },
        {1, 2,  1,   0,    0,     0,      0,       0,        0        },
        {1, 2,  5,   29,   118,   451,    1452,    4785,     12074    },
        {1, 6,  16,  26,   41,    19,     6,       0,        0        },
        {1, 9,  26,  71,   140,   284,    357,     338,      194      },
        {1, 0,  0,   0,    0,     0,      0,       0}
	};

	for (int i = 0; i < fens.size(); i++) {
		std::cout << "====== Testing set " << i << " ======\n";
		std::cout << "FEN: " << fens[i] << "\n\n";

		game.setFromFen(fens[i]);

		for (int j = 0; j < results[i].size(); j++) {
			int res = perft(game, j);

			std::cout << "Perft with depth " << j << ": " << res;

			if (res == results[i][j]) {
				std::cout << " (passed)\n";
			} else if (res > results[i][j]) {
				std::cout << " (failed over, expecting " << results[i][j] << ")\n";
			} else {
				std::cout << " (failed under, expecting " << results[i][j] << ")\n";
			}
		}
	}
}

int main() {
	Game game;

	runperfttests(game);
}