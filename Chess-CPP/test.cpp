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

	if (token == "piece")
	{
		cout << "please enter a valid square" << endl;
		return false;
	}

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

	cout << "is piece absolute pinned? " << (is_pinned ? "yes" : "no") << endl;
}

void Test::log_piece_is_hanging()
{
	bool is_hanging = is_piece_hanging_on_square(pos, string_to_square(token));

	cout << "is piece hanging? " << (is_hanging ? "yes" : "no") << endl;
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

	vector<pair<string, int>> move_evaluations = get_move_evaluations(pos, states);

	for (pair<string, int> item : move_evaluations)
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

	vector<pair<string, int>> move_evaluations = get_move_evaluations(pos, states);

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
	read_next_token();

	if (!is_move_token_valid()) return;

	cout << "(1) "; // Move Function Idea 1
	log_move_centipawn_loss();

	cout << "(2) "; // Move Function Idea 2
	log_move_is_hanging_capture();

	cout << "(3) "; // Move Function Idea 3
	log_is_winning_static_exchange_evaluation();

	cout << "(4) "; // Move Function Idea 4
	log_does_allow_static_exchange_evaluation();

	cout << "(5) "; // Move Function Idea 5
	log_does_permit_good_move();

	cout << "(6) "; // Move Function Idea 6
	log_does_permit_good_move_on_square();
}

bool Test::is_move_token_valid()
{
	if (token == "move")
	{
		cout << "please enter a valid move" << endl;
		return false;
	}

	if (UCI::to_move(*pos, token)) return true;
	
	cout << token << " is not a valid move" << endl;

	return false;
}

void Test::log_move_centipawn_loss()
{
	int loss = get_centipawn_loss(pos, states, token);

	if (loss == 0) cout << "no centipawn loss, bestmove found by engine" << endl;
	else cout << "centipawn loss of " << loss << endl;
}

void Test::log_move_is_hanging_capture()
{
	Move move = UCI::to_move(*pos, token);

	bool is_hanging = is_piece_hanging_on_square(pos, to_sq(move));

	cout << ((is_hanging) ? "yes, does " : "no, does not ");
	cout << "capture a hanging piece on " << token[2] << token[3] << endl;
}

void Test::log_is_winning_static_exchange_evaluation()
{
	Move move = UCI::to_move(*pos, token);

	MoveList captures = MoveList<CAPTURES>(*pos);

	if (!captures.contains(move))
	{
		cout << "no, Static Exchange Evaluation does not apply, move is not a capture" << endl;
		return;
	}

	Value threshold = PawnValueMg;
	if (pos->see_ge(move, threshold))
	{
		cout << "yes, according to Static Exchange Evaluation, this is a winning capture" << endl;
	}
	else
	{
		cout << "no, according to Static Exchange Evaluation, ";
		cout << "this is not a winning capture" << endl;
	}
}

void Test::log_does_allow_static_exchange_evaluation()
{
	string originalFen = pos->fen();

	Move token_move = UCI::to_move(*pos, token);
	make_move(pos, states, token_move);

	MoveList legal_moves = MoveList<LEGAL>(*pos);
	MoveList captures = MoveList<CAPTURES>(*pos);

	bool found_see = false;

	for (ExtMove move : captures)
	{
		if (legal_moves.contains(move))
		{
			Value threshold = PawnValueMg;
			if (pos->see_ge(move, threshold))
			{
				if (!found_see)
				{
					found_see = true;
					cout << "yes, the opponent can make a winning capture according to Static ";
					cout << "Exchange Evaluation through any of these moves:" << endl;
				}
				cout << "\t"<< move_to_string(move) << endl;
			}
		}
	}

	if (!found_see)
	{
		cout << "no, move does not allow the opponent to make a winning capture ";
		cout << "according to Static Exchange Evaluation" << endl;
	}
	set_position(pos, states, originalFen);
}

void Test::log_does_permit_good_move()
{
	int permitCentipawnLoss = 200;

	int loss = get_centipawn_loss(pos, states, token);

	if (loss >= permitCentipawnLoss)
	{
		Move move = UCI::to_move(*pos, token);

		cout << "yes, this move permits a good move: ";
		cout << get_best_move_evaluation_after_given_move(pos, states, move).first << endl;
	}
	else
	{
		cout << "no, this move does not permit a good move" << endl;
	}
}

void Test::log_does_permit_good_move_on_square()
{
	cerr << "no implementation" << endl;
}

#pragma endregion
