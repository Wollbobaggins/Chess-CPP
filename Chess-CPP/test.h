// TODO: rename some of these
// TODO: adhere to c++ naming conventions (data members should be camel cased)

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

	// foundational functions
	bool read_next_token();
	void log_help();
	
	// piece functions
	void run_piece_function_ideas();
	bool is_piece_token_valid();
	void log_piece_legal_moves();
	void log_piece_captures();
	void log_piece_is_pinned();
	void log_piece_is_hanging();
	void log_legal_moves_for_capturable_pieces();

	// position functions
	void run_position_function_ideas();

	// move functions
	void run_move_function_ideas();
};

