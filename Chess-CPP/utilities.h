#pragma once
//TODO: are there excess includes?
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

namespace Utilities
{
#pragma region Conversion Functions

	string move_to_string(ExtMove move)
	{
		return UCI::square(from_sq(move)) + UCI::square(to_sq(move));
	}

	string square_to_string(Square square)
	{
		// HACK: this code is redundant, but it makes things less of a headache

		return UCI::square(square);
	}

	Square string_to_square(string& str)
	{
		if (str.size() != 2) return SQ_NONE;

		string alphabet = "abcdefgh";
		string numbers = "12345678";

		File f = static_cast<File>(alphabet.find(str.at(0)));
		Rank r = static_cast<Rank>(numbers.find(str.at(1)));

		return make_square(f, r);
	}

#pragma endregion

#pragma region Chess Utility Functions

	Piece get_piece_on_square(Position* pos, Square square)
	{
		return pos->piece_on(square);
	}

	bool is_check(Position* pos)
	{
		return pos->checkers();
	}

	bool is_checkmate(Position* pos)
	{
		MoveList legal_moves = MoveList<LEGAL>(*pos);

		return !legal_moves.size();
	}

	void set_side_to_move(Position* pos, StateListPtr* states, Color color)
	{

	}

	void set_position(Position* pos, StateListPtr* states, string& fen)
	{
		// code copied from UCI::position(Position& pos, istringstream& is, StateListPtr& states)
		*states = StateListPtr(new std::deque<StateInfo>(1)); // Drop old and create a new one
		pos->set(fen, Options["UCI_Chess960"], &(*states)->back(), Threads.main());
	}

#pragma endregion

#pragma region Engine Functions

	int evaluate_move(Position* pos, StateListPtr* states, string& move)
	{
		Search::LimitsType limits;
		limits.depth = 3;
		limits.searchmoves.push_back(UCI::to_move(*pos, move));

		string output = "";

		// create a new stringbuf for the threads & associate with std::cout
		stringbuf stringBuffer(ios::out);
		streambuf* oldStringBuffer = cout.rdbuf(&stringBuffer);

		cout << "other" << endl;

		// start searching
		Threads.start_thinking(*pos, *states, limits, false);
		while (!Threads.stop) { /* wait for search */ }

		//// finished thinking, but now wait for main thread to log bestmove & copy current messages
		//while (output.find("bestmove") == string::npos) { output = stringBuffer.str(); }

		// restore cout's original buffer
		cout.rdbuf(oldStringBuffer);

		//cout << output << endl; // print out the modified output

		cout << "WOO done!" << endl;

		return 0;
	}

#pragma endregion

#pragma region Debug Functions

	void debug_log_bitboard(Bitboard b)
	{
		// this function has been copied from Bitboard::pretty

		std::string s = "+---+---+---+---+---+---+---+---+\n";

		for (Rank r = RANK_8; r >= RANK_1; --r)
		{
			for (File f = FILE_A; f <= FILE_H; ++f)
				s += b & make_square(f, r) ? "| X " : "|   ";

			s += "| " + std::to_string(1 + r) + "\n+---+---+---+---+---+---+---+---+\n";
		}
		s += "  a   b   c   d   e   f   g   h\n";

		cout << s << endl;
	}

	string exec(string command) {
		char buffer[128];
		string result = "";

		// Open pipe to file
		FILE* pipe = _popen(command.c_str(), "r");
		if (!pipe) {
			return "_popen failed!";
		}

		// read till end of process:
		while (!feof(pipe)) {

			// use buffer to read and add to result
			if (fgets(buffer, 128, pipe) != NULL)
				result += buffer;
		}

		_pclose(pipe);
		return result;
	}

#pragma endregion

}
