#include "test.h"
#include "utilities.h"

using namespace Utilities;

#pragma region Foundational Code

void Test::run(Position& _pos, istringstream& _is, StateListPtr& _states)
{
	pos = &_pos;
	is = &_is;
	states = &_states;
	token = "";

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

	// Piece Function Idea 1
	log_piece_legal_moves(); 

	// Piece Function Idea 2
	log_piece_captures(); 

	// Piece Function Idea 3
	log_piece_is_pinned(); 

	// Piece Function Idea 4
	log_piece_is_hanging(); 

	// Piece Function Idea 5
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

void Test::log_piece_legal_moves()
{
	if (is_checkmate(pos))
	{
		cout << "no legal moves, it is checkmate" << endl;
		return;
	}
	//TODO: the code above and below has been copied 3 times in 3 different functions
	Square square = string_to_square(token);
	Piece piece = get_piece_on_square(pos, square);
	Color piece_color = (piece == NO_PIECE) ? COLOR_NB : color_of(piece);

	if (pos->side_to_move() != piece_color)
	{
		cout << "piece cannot move, it is not this color's turn yet" << endl;
		return;
	}

	MoveList legal_moves = MoveList<LEGAL>(*pos);
	int legal_move_count = 0;

	for (ExtMove m : legal_moves)
	{
		if (from_sq(m) == square) legal_move_count++;
	}

	cout << "piece on " << token << " has " << legal_move_count << " legal moves:" << endl;

	for (ExtMove m : legal_moves)
	{
		if (from_sq(m) == square) cout << "\t" << move_to_string(m) << endl;
	}
}

void Test::log_piece_captures()
{
	if (is_checkmate(pos))
	{
		cout << "no legal captures, it is checkmate" << endl;
		return;
	}

	Square square = string_to_square(token);
	Piece piece = get_piece_on_square(pos, square);
	Color piece_color = (piece == NO_PIECE) ? COLOR_NB : color_of(piece);

	if (pos->side_to_move() != piece_color)
	{
		cout << "piece cannot capture, it is not this color's turn yet" << endl;
		return;
	}

	MoveList legal_moves = MoveList<LEGAL>(*pos);
	MoveList captures = MoveList<CAPTURES>(*pos);
	int capture_count = 0;

	for (ExtMove m : captures)
	{
		if (from_sq(m) == square && legal_moves.contains(m)) capture_count++;
	}

	cout << "piece on " << token << " has " << capture_count << " captures:" << endl;

	for (ExtMove m : captures)
	{
		if (from_sq(m) == square && legal_moves.contains(m))
			cout << "\t" << move_to_string(m) << endl;
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
	if (is_checkmate(pos))
	{
		cout << "no legal moves for capturable pieces, it is checkmate" << endl;
		return;
	}

	Square square = string_to_square(token);
	Piece piece = get_piece_on_square(pos, square);
	Color piece_color = (piece == NO_PIECE) ? COLOR_NB : color_of(piece);

	if (pos->side_to_move() != piece_color)
	{
		cout << "piece cannot capture, it is not this color's turn yet" << endl;
		return;
	}

	MoveList legal_moves = MoveList<LEGAL>(*pos);
	MoveList captures = MoveList<CAPTURES>(*pos);

	// Change who's move it is
	string current_fen = pos->fen();
	string fen = "rnbqkbnr/pppp1ppp/8/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2";
	//set_position(pos, states, fen);
	cout << current_fen << endl;

	//for (ExtMove m : captures)
	//{
	//	if (from_sq(m) == square && legal_moves.contains(m))
	//	{
	//		token = square_to_string(to_sq(m));
	//		log_piece_legal_moves();
	//	}
	//}

	// Change back to original move
}

#pragma endregion

#pragma region Position Function Ideas

void Test::run_position_function_ideas()
{
	log_best_moves();
}

void Test::log_best_moves()
{
	MoveList legal_moves = MoveList<LEGAL>(*pos);
	map<string, int> best_moves;
	string move_string;

	for (ExtMove move : legal_moves)
	{
		move_string = move_to_string(move);
		best_moves[move_string] = evaluate_move(pos, states, move_string);
	}

	for (auto item : best_moves)
	{
		cout << "\t" << item.first << ": " << item.second << endl;
	}

	// get all legal moves

	// for each legal move, evaluate the position
	//string str = "e2e4";
	//limits.searchmoves.clear();
	//limits.searchmoves.push_back(UCI::to_move(*pos, str));

}

void Test::log_threat_moves()
{

}

#pragma endregion

#pragma region Move Functions Ideas

void Test::run_move_function_ideas()
{
	cerr << "no implementation" << endl;
	//cout << Threads.main()->id() << Threads.main()->exit << Threads.main()->id() << endl;
}

#pragma endregion
