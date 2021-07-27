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
	void log_piece_legal_moves(); // Piece Function Idea 1
	void log_piece_captures(); // Piece Function Idea 2
	void log_piece_is_pinned(); // Piece Function Idea 3
	void log_piece_is_hanging(); // Piece Function Idea 4
	void log_legal_moves_for_capturable_pieces(); // Piece Function Idea 5

	// position functions
	void run_position_function_ideas();
	void log_best_moves(); // Position Function Idea 1
	void log_threat_moves(); // Position Function Idea 2

	// move functions
	void run_move_function_ideas(); 
	bool is_move_token_valid();
	void log_move_centipawn_loss(); // Move Function Idea 1
	void log_move_is_hanging_capture(); // Move Function Idea 2
	void log_is_winning_static_exchange_evaluation(); // Move Function Idea 3
	void log_does_allow_static_exchange_evaluation(); // Move Function Idea 4
	void log_does_permit_good_move(); // Move Function Idea 5
	void log_does_permit_good_move_by_undefending_square(); // Move Function Idea 6
	void log_is_move_discovered_attack(); // Additional Move Function Idea 
};

