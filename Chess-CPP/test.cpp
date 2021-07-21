#include "test.h"
#include "utilities.h"

using namespace Utilities;

#pragma region Foundational Code

void Test::run(Position& _pos, istringstream& _is, StateListPtr& _states)
{
	pos = &_pos;
	is = &_is;
	states = &_states;

	if (read_next_token())
	{
		if (token == "piece") run_piece_function_ideas();
		else if (token == "position") run_position_function_ideas();
		else if (token == "move") run_move_function_ideas();
		else
		{
			cout << "Unknown command: " << token << endl;
			log_help();
		}
	}
	else
	{
		log_help();
	}
}

bool Test::read_next_token()
{
	*is >> token;

	return !token.empty();
}

void Test::log_help()
{
	cout << "here is a list of availble commands for test:" << endl;

	cout << "\ttest piece -arg : a square in FILE/RANK form" << endl;
	cout << "\ttest position -arg : number of moves to display" << endl;
	cout << "\ttest move -arg : a move in FILE/RANK/FILE/RANK form" << endl;
}

#pragma endregion

#pragma region Piece Functions Ideas

void Test::run_piece_function_ideas()
{
	read_next_token(); 

	if (!is_piece_token_valid()) return;

	MoveList legal_moves = MoveList<LEGAL>(*pos);
	log_piece_moves(legal_moves);

	MoveList capture_moves = MoveList<CAPTURES>(*pos);
	log_piece_moves(capture_moves);

	log_piece_is_pinned();
	log_piece_is_hanging();

	log_legal_moves_for_capturable_pieces();
}

bool Test::is_piece_token_valid()
{
	Square square = string_to_square(token);

	if (square == SQ_NONE)
	{
		cout << token << " is not a valid square" << endl;
		return false;
	}

	if (get_piece_on_square(pos, square) == NO_PIECE)
	{
		cout << "there is no piece on " << token << endl;
		return false;
	}

	return true;
}

template<GenType Type>
void Test::log_piece_moves(MoveList<Type>& list)
{
	if (is_checkmate(pos))
	{
		cout << "no legal moves, checkmate" << endl;
		return;
	}

	Square square = string_to_square(token);
	Piece piece = get_piece_on_square(pos, square);
	Color piece_color = (piece == NO_PIECE) ? COLOR_NB : color_of(piece);
	
	if (pos->side_to_move() != piece_color)
	{
		cout << "piece cannot ";
		cout << ((Type == LEGAL) ? "move" : "capture");
		cout << ", it is not this color's turn yet" << endl;
		return;
	}

	int legal_move_count = 0;

	for (ExtMove m : list)
	{
		if (from_sq(m) == square) legal_move_count++;
	}

	cout << "piece on " << token << " has " << legal_move_count;
	cout << ((Type == LEGAL) ? " legal moves:" : " captures:") << endl;

	for (ExtMove m : list)
	{
		if (from_sq(m) == square) cout << "\t" << move_to_string(m) << endl;
	}
}

void Test::log_piece_is_pinned()
{
	Square square = string_to_square(token);
	Piece piece = get_piece_on_square(pos, square);
	Color piece_color = color_of(piece);

	Bitboard block_board = pos->blockers_for_king(piece_color);

	bool is_pinned = block_board & square;

	cout << "is piece absolute pinned? ";
	cout << (is_pinned ? "yes" : "no") << endl;
}

void Test::log_piece_is_hanging()
{
	Square square = string_to_square(token);
	Piece piece = get_piece_on_square(pos, square);
	Color piece_color = color_of(piece);

	Bitboard ally_piece_board = pos->pieces(piece_color);
	Bitboard enemy_piece_board = pos->pieces(~piece_color);
	Bitboard attack_board = pos->attackers_to(square);

	bool is_defended = ally_piece_board & attack_board;
	bool is_attacked = enemy_piece_board & attack_board;

	bool is_hanging = !is_defended && is_attacked;

	cout << "is piece hanging? ";
	cout << (is_hanging ? "yes" : "no") << endl;
}

void Test::log_legal_moves_for_capturable_pieces()
{
	// get capture moves for piece
	// if (has captures)
		// change who's turn it is of fen/position
		// foreach capturable piece
			// get legal moves

	//debug_log_bitboard(pos->checkers());
	//debug_log_bitboard(pos->blockers_for_king(~pos->side_to_move()));

	//MoveList legal_moves = MoveList<LEGAL>(*pos);
	//log_piece_moves(legal_moves);
}

#pragma endregion

#pragma region Position Function Ideas

void Test::run_position_function_ideas()
{
	cerr << "no implementation" << endl;
}

#pragma endregion

#pragma region Move Functions Ideas

void Test::run_move_function_ideas()
{
	cerr << "no implementation" << endl;
}

#pragma endregion
