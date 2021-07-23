#pragma once
//TODO: are there excess includes?
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include <chrono>
#include <thread>

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
using namespace chrono_literals;

namespace Utilities
{
#pragma region Misc Utility Functions

	bool compare_move_evaluations(pair<string, int>& a, pair<string, int>& b)
	{
		return a.second > b.second;
	}

	void sort_move_evaluations(vector<pair<string, int>>& move_evaluations)
	{
		sort(move_evaluations.begin(), move_evaluations.end(), compare_move_evaluations);
	}

	string get_next_word(string& text, int& index)
	{
		string output = "";
		int startIndex = index;
		int length = text.length() - index;
		for (char s : text.substr(startIndex, length))
		{
			index++;
			if (s == ' ') return output;
			output += s;
		}
	}

	int get_evaluation_from_output(string& output, int depth)
	{
		int index = output.find("depth " + to_string(depth));

		while (get_next_word(output, index) != "score") {}

		string cp = get_next_word(output, index);
		int score;

		if (cp == "cp") score = stoi(get_next_word(output, index));
		else score = VALUE_INFINITE * stoi(get_next_word(output, index));

		return score;
	}

#pragma endregion

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

		size_t index = alphabet.find(str.at(0));
		if (index == -1) return SQ_NONE;
		File f = static_cast<File>(index);

		index = numbers.find(str.at(1));
		if (index == -1) return SQ_NONE;
		Rank r = static_cast<Rank>(index);

		return make_square(f, r);
	}

	string int_to_string_evaluation(int eval)
	{
		if (abs(eval) >= VALUE_INFINITE) return "mate in " + to_string(eval / VALUE_INFINITE);
		else return to_string(eval) + " centipawns";
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

	void change_side_to_move(Position* pos, StateListPtr* states)
	{
		string fen = pos->fen();

		int index = 0;
		get_next_word(fen, index);

		fen[index] = (pos->side_to_move() == WHITE) ? 'b' : 'w';

		*states = StateListPtr(new deque<StateInfo>(1));
		pos->set(fen, Options["UCI_Chess960"], &(*states)->back(), Threads.main());
	}

#pragma endregion

#pragma region Engine Functions

	vector<pair<string, int>> get_best_moves(Position* pos, StateListPtr* states)
	{
		MoveList legal_moves = MoveList<LEGAL>(*pos);
		vector<pair<string, int>> best_moves;
		string move_string;

		Search::LimitsType limits;
		limits.depth = 5; // HACK: hardcoded depth for engine

		// create a new stringbuf for the threads & associate with std::cout
		stringbuf stringBuffer(ios::out);
		streambuf* oldStringBuffer = cout.rdbuf(&stringBuffer);
		string output = "";

		for (ExtMove move : legal_moves)
		{
			move_string = move_to_string(move);
			limits.searchmoves.clear();
			limits.searchmoves.push_back(move);

			output = "";
			stringBuffer.str(output);

			// start searching
			Threads.start_thinking(*pos, *states, limits, false);
			while (!Threads.stop) { /* wait for search */ }

			this_thread::sleep_for(1ms);

			output = stringBuffer.str();

			pair<string, int> p(move_string, get_evaluation_from_output(output, limits.depth));

			best_moves.push_back(p);
		}

		// restore cout's original buffer
		cout.rdbuf(oldStringBuffer);

		return best_moves;
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
