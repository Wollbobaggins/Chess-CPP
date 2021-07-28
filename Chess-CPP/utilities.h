#pragma once

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
		if (abs(a.second) >= VALUE_INFINITE && abs(b.second) >= VALUE_INFINITE) 
		{
			if ((a.second > 0 && b.second > 0) || (a.second < 0 && b.second < 0)) 
			{
				return a.second < b.second;
			}
		}
		return a.second > b.second;
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
		return output;
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

	Square string_to_from_square(string& str)
	{
		if (str.size() != 4) return SQ_NONE;

		string squareString = str.substr(0, 1) + str.substr(1, 1);
		return string_to_square(squareString);
	}

	Square string_to_to_square(string& str)
	{
		if (str.size() != 4) return SQ_NONE;
		string squareString = str.substr(2, 1) + str.substr(3, 1);
		return string_to_square(squareString);
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
		MoveList legalMoves = MoveList<LEGAL>(*pos);

		return !legalMoves.size();
	}

	void set_position(Position* pos, StateListPtr* states, string& fen)
	{
		*states = StateListPtr(new deque<StateInfo>(1));
		pos->set(fen, Options["UCI_Chess960"], &(*states)->back(), Threads.main());
	}

	void change_side_to_move(Position* pos, StateListPtr* states)
	{
		string fen = pos->fen();

		int index = 0;
		get_next_word(fen, index);

		fen[index] = (pos->side_to_move() == WHITE) ? 'b' : 'w';

		set_position(pos, states, fen);
	}

	void make_move(Position* pos, StateListPtr* states, Move move)
	{
		string fen = pos->fen();
		set_position(pos, states, fen);

		(*states)->emplace_back();
		pos->do_move(move, (*states)->back());
	}

	bool is_square_attacked_by_color(Position* pos, Square square, Color color)
	{
		// does this color's pieces overlap on these attacked squares
		return pos->pieces(color) & pos->attackers_to(square);
	}

	bool is_piece_hanging_on_square(Position* pos, Square square)
	{
		Piece piece = get_piece_on_square(pos, square);
		if (get_piece_on_square(pos, square) == NO_PIECE) return false;

		Color pieceColor = color_of(piece);

		bool isDefended = is_square_attacked_by_color(pos, square, pieceColor);
		bool isAttacked = is_square_attacked_by_color(pos, square, ~pieceColor);

		return !isDefended && isAttacked;
	}

	bool is_move_defended_against(Position* pos, Move move)
	{
		return !pos->see_ge(move, VALUE_ZERO);
	}

	vector<ExtMove> get_discovered_attacks_after_move(
		Position* pos, StateListPtr* states, Move move)
	{
		vector<ExtMove> discoveredAttacks;
		string originalFen = pos->fen();

		MoveList captures = MoveList<CAPTURES>(*pos);

		make_move(pos, states, move);
		change_side_to_move(pos, states);

		MoveList newCaptures = MoveList<CAPTURES>(*pos);

		set_position(pos, states, originalFen);

		for (ExtMove capture : newCaptures)
		{
			if (captures.contains(capture) || from_sq(capture) == to_sq(move)) continue;

			discoveredAttacks.push_back(capture);
		}

		return discoveredAttacks;
	}

#pragma endregion

#pragma region Engine Functions

	vector<pair<string, int>> get_move_evaluations(Position* pos, StateListPtr* states, int depth)
	{
		MoveList legalMoves = MoveList<LEGAL>(*pos);
		vector<pair<string, int>> moveEvaluations;
		string moveString;

		Search::LimitsType limits;
		limits.depth = depth; 

		// create a new stringbuf for the threads & associate with std::cout
		stringbuf stringBuffer(ios::out);
		streambuf* oldStringBuffer = cout.rdbuf(&stringBuffer);
		string output = "";

		for (ExtMove move : legalMoves)
		{
			moveString = move_to_string(move);
			limits.searchmoves.clear();
			limits.searchmoves.push_back(move);

			output = "";
			stringBuffer.str(output);

			// start searching
			Threads.start_thinking(*pos, *states, limits, false);
			while (!Threads.stop) { /* wait for search */ }

			this_thread::sleep_for(1ms);

			output = stringBuffer.str();

			pair<string, int> p(moveString, get_evaluation_from_output(output, limits.depth));

			moveEvaluations.push_back(p);
		}

		// restore cout's original buffer
		cout.rdbuf(oldStringBuffer);

		sort(moveEvaluations.begin(), moveEvaluations.end(), compare_move_evaluations);

		return moveEvaluations;
	}

	int get_centipawn_loss(vector<pair<string, int>>& moveEvaluations, string& token)
	{
		for (pair<string, int> item : moveEvaluations)
		{
			if (item.first != token) continue;

			return moveEvaluations[0].second - item.second;
		}

		cerr << "token is invalid" << endl;
		return 0;
	}

	vector<pair<string, int>> get_permitted_good_moves(Position* pos, StateListPtr* states, 
		int depth, vector<pair<string, int>>& moveEvaluations, int permitCentipawnLoss, Move move)
	{
		string originalFen = pos->fen();

		make_move(pos, states, move);

		vector<pair<string, int>>  nextMoveEvaluations = get_move_evaluations(pos, states, depth);

		set_position(pos, states, originalFen);

		int positionEvaluation = moveEvaluations[0].second;
		int nextPositionEvaluation = nextMoveEvaluations[0].second;

		for (int i = nextMoveEvaluations.size() - 1; i >= 0; i--)
		{
			if (positionEvaluation + nextMoveEvaluations[i].second >= permitCentipawnLoss 
				&& nextPositionEvaluation - nextMoveEvaluations[i].second <= permitCentipawnLoss) 
				continue;

			nextMoveEvaluations.erase(nextMoveEvaluations.begin() + i);
		}

		return nextMoveEvaluations;
	} 

#pragma endregion

#pragma region Debug Functions

	void debug_log_bitboard(Bitboard b)
	{
		cout << Bitboards::pretty(b) << endl;
	}

#pragma endregion

}
