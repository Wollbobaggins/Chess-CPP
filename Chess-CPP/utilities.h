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

#pragma region General Utility Functions

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

#pragma endregion

}
