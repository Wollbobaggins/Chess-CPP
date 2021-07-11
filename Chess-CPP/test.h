#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "Stockfish src/evaluate.h"
#include "Stockfish src/movegen.h"
#include "Stockfish src/position.h"
#include "Stockfish src/search.h"
#include "Stockfish src/thread.h"
#include "Stockfish src/timeman.h"
#include "Stockfish src/tt.h"
#include "Stockfish src/uci.h"
#include "Stockfish src/syzygy/tbprobe.h"

using namespace std;
using namespace Stockfish;

class Test
{
public:
	void run(Position& _pos, istringstream& _is, StateListPtr& _states);

private:
	Position* pos;
	istringstream* is;
	StateListPtr* states;
	string token;

	bool read_next_token();
	void log_help();

	void run_piece_function_ideas();
	void log_piece_legal_moves();

	void run_position_function_ideas();

	void run_move_function_ideas();
};

