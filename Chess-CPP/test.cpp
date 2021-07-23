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

	cout << "(1) "; // Piece Function Idea 1
	log_piece_legal_moves(); 

	cout << "(2) "; // Piece Function Idea 2
	log_piece_captures(); 

	cout << "(3) "; // Piece Function Idea 3
	log_piece_is_pinned(); 

	cout << "(4) "; // Piece Function Idea 4
	log_piece_is_hanging(); 

	cout << "(5) "; // Piece Function Idea 5
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

	for (ExtMove move : legal_moves)
	{
		if (from_sq(move) == square) legal_move_count++;
	}

	cout << "piece on " << token << " has " << legal_move_count << " legal moves:" << endl;

	for (ExtMove move : legal_moves)
	{
		if (from_sq(move) == square) cout << "\t" << move_to_string(move) << endl;
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

	for (ExtMove move : captures)
	{
		if (from_sq(move) == square && legal_moves.contains(move)) capture_count++;
	}

	cout << "piece on " << token << " has " << capture_count << " captures:" << endl;

	for (ExtMove move : captures)
	{
		if (from_sq(move) == square && legal_moves.contains(move))
			cout << "\t" << move_to_string(move) << endl;
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

	cout << "logging legal moves for capturable pieces" << endl;

	MoveList legal_moves = MoveList<LEGAL>(*pos);
	MoveList captures = MoveList<CAPTURES>(*pos);

	change_side_to_move(pos, states);

	for (ExtMove move : captures)
	{
		if (from_sq(move) == square && legal_moves.contains(move))
		{
			token = square_to_string(to_sq(move)); // HACK: we can edit token, last time it's used
			log_piece_legal_moves();
		}
	}

	change_side_to_move(pos, states);
}

#pragma endregion

#pragma region Position Function Ideas

void Test::run_position_function_ideas()
{
	read_next_token();

	cout << "(1) "; // Position Function Idea 1
	log_best_moves();

	cout << "(2) "; // Position Function Idea 2
	log_threat_moves();
}

void Test::log_best_moves()
{
	int n = 0;
	try
	{
		n = stoi(token);
		cout << "logging best " << n << " moves" << endl;
	}
	catch (invalid_argument& e)
	{
		// if no conversion could be performed
		cout << "unrecognized value of N, logging all best moves" << endl;
		n = 10000;
	}

	vector<pair<string, int>> move_evaluations = get_best_moves(pos, states);
	sort_move_evaluations(move_evaluations);

	for (auto item : move_evaluations)
	{
		if (n-- == 0) break;

		cout << "\t" << item.first << ": " << int_to_string_evaluation(item.second) << endl;
	}

	if (n != 0) cout << "\tno more moves found" << endl;
}

void Test::log_threat_moves()
{
	int n = 0;
	try
	{
		n = stoi(token);
		cout << "logging threat " << n << " moves" << endl;
	}
	catch (invalid_argument& e)
	{
		// if no conversion could be performed
		cout << "unrecognized value of N, logging all threat moves" << endl;
		n = 10000;
	}

	vector<pair<string, int>> move_evaluations = get_best_moves(pos, states);
	sort_move_evaluations(move_evaluations);

	for (int i = move_evaluations.size() - 1; i >= 0; i--)
	{
		if (n-- == 0) break;

		cout << "\t" << move_evaluations[i].first << ": ";
		cout << int_to_string_evaluation(move_evaluations[i].second) << endl;
	}

	if (n != 0) cout << "\tno more moves found" << endl;
}

#pragma endregion

#pragma region Move Functions Ideas

void Test::run_move_function_ideas()
{
	cerr << "no implementation" << endl;
}

#pragma endregion
