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
		if (token == "settings") cout << "settings menu not yet implemented" << endl;
		else if (token == "piece") run_piece_function_ideas();
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
	Square square = string_to_square(token);
	Piece piece = get_piece_on_square(pos, square);
	Color pieceColor = (piece == NO_PIECE) ? COLOR_NB : color_of(piece);

	if (pos->side_to_move() != pieceColor)
	{
		cout << "piece cannot move, it is not this color's turn yet" << endl;
		return;
	}

	MoveList legalMoves = MoveList<LEGAL>(*pos);
	int legalMoveCount = 0;

	for (ExtMove move : legalMoves)
	{
		if (from_sq(move) == square) legalMoveCount++;
	}

	cout << "piece on " << token << " has " << legalMoveCount << " legal moves:" << endl;

	for (ExtMove move : legalMoves)
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
	Color pieceColor = (piece == NO_PIECE) ? COLOR_NB : color_of(piece);

	if (pos->side_to_move() != pieceColor)
	{
		cout << "piece cannot capture, it is not this color's turn yet" << endl;
		return;
	}

	MoveList legalMoves = MoveList<LEGAL>(*pos);
	MoveList captures = MoveList<CAPTURES>(*pos);
	int captureCount = 0;

	for (ExtMove move : captures)
	{
		if (from_sq(move) == square && legalMoves.contains(move)) captureCount++;
	}

	cout << "piece on " << token << " has " << captureCount << " captures:" << endl;

	for (ExtMove move : captures)
	{
		if (from_sq(move) == square && legalMoves.contains(move))
			cout << "\t" << move_to_string(move) << endl;
	}
}

void Test::log_piece_is_pinned()
{
	Square square = string_to_square(token);
	Piece piece = get_piece_on_square(pos, square);
	Color pieceColor = color_of(piece);

	Bitboard blockBoard = pos->blockers_for_king(pieceColor);

	bool isPinned = blockBoard & square;

	cout << "is piece absolute pinned? " << (isPinned ? "yes" : "no") << endl;
}

void Test::log_piece_is_hanging()
{
	bool isHanging = is_piece_hanging_on_square(pos, string_to_square(token));

	cout << "is piece hanging? " << (isHanging ? "yes" : "no") << endl;
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
	Color pieceColor = (piece == NO_PIECE) ? COLOR_NB : color_of(piece);

	if (pos->side_to_move() != pieceColor)
	{
		cout << "piece cannot capture, it is not this color's turn yet" << endl;
		return;
	}

	// FIXME: do not log if there are no moves
	cout << "logging legal moves for capturable pieces" << endl; 

	MoveList legalMoves = MoveList<LEGAL>(*pos);
	MoveList captures = MoveList<CAPTURES>(*pos);

	change_side_to_move(pos, states);

	for (ExtMove move : captures)
	{
		if (from_sq(move) == square && legalMoves.contains(move))
		{
			token = square_to_string(to_sq(move)); // we can edit token, last time it's used
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

	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states);

	for (pair<string, int> item : moveEvaluations)
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

	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states);

	for (int i = moveEvaluations.size() - 1; i >= 0; i--)
	{
		if (n-- == 0) break;

		cout << "\t" << moveEvaluations[i].first << ": ";
		cout << int_to_string_evaluation(moveEvaluations[i].second) << endl;
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
	log_does_permit_good_move_by_undefending_square();

	cout << "(+) "; // Additional Move Function Idea
	log_is_move_discovered_attack();
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
	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states);
	int loss = get_centipawn_loss(moveEvaluations, token);

	if (loss == 0) cout << "no centipawn loss, bestmove found by engine" << endl;
	else if (abs(loss) >= VALUE_INFINITE)  cout << "massive centipawn loss, missed mate!" << endl;
	else cout << "centipawn loss of " << loss << endl;

	// FIXME: centipawn loss does not always output checkmate when lost
	// FIXME: change cout given if it's a good mate or a bad mate
}

void Test::log_move_is_hanging_capture()
{
	Move move = UCI::to_move(*pos, token);

	bool isHanging = is_piece_hanging_on_square(pos, to_sq(move));

	cout << ((isHanging) ? "yes, does " : "no, does not ");
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

	Value threshold;
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

	MoveList legalMoves = MoveList<LEGAL>(*pos);
	MoveList captures = MoveList<CAPTURES>(*pos);

	bool foundMove = false;

	for (ExtMove move : captures)
	{
		if (legalMoves.contains(move))
		{
			Value threshold = PawnValueMg;
			if (pos->see_ge(move, threshold))
			{
				if (!foundMove)
				{
					foundMove = true;
					cout << "yes, the opponent can make a winning capture according to Static ";
					cout << "Exchange Evaluation through any of these moves:" << endl;
				}
				cout << "\t"<< move_to_string(move) << endl;
			}
		}
	}

	if (!foundMove)
	{
		cout << "no, move does not allow the opponent to make a winning capture ";
		cout << "according to Static Exchange Evaluation" << endl;
	}
	set_position(pos, states, originalFen);
}

void Test::log_does_permit_good_move()
{
	int permitCentipawnLoss = 200; // HACK: hardcoded permit threshold

	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states);

	if (get_centipawn_loss(moveEvaluations, token) < permitCentipawnLoss)
	{
		cout << "no, this move does not permit a good move" << endl;
		return;
	}

	bool foundMove = false;

	Move token_move = UCI::to_move(*pos, token);

	vector<pair<string, int>> permitted_good_moves = get_permitted_good_moves(
		pos, states, moveEvaluations, permitCentipawnLoss, token_move);

	for (pair<string, int> item : permitted_good_moves)
	{
		if (!foundMove)
		{
			foundMove = true;
			cout << "yes, this move permits the following good moves: " << endl;
		}

		cout << "\t" << item.first << ": " << int_to_string_evaluation(-item.second) << endl;
	}

}

void Test::log_does_permit_good_move_by_undefending_square()
{
	bool foundMove = false;
	int permitCentipawnLoss = 200; // HACK: hardcoded permit threshold

	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states);

	if (get_centipawn_loss(moveEvaluations, token) < permitCentipawnLoss)
	{
		cout << "no, this move does not permit a good move ";
		cout << "by undefending a specific square" << endl;
		return;
	}

	Color color = pos->side_to_move();
	Move tokenMove = UCI::to_move(*pos, token);

	vector<pair<string, int>> permitted_good_moves = get_permitted_good_moves(
		pos, states, moveEvaluations, permitCentipawnLoss, tokenMove);

	for (pair<string, int> item : permitted_good_moves)
	{	
		Square square = string_to_to_square(item.first);

		// the square can't already be under-defended before the move was made
		change_side_to_move(pos, states);
		Move move = UCI::to_move(*pos, item.first);
		if (!is_move_defended_against(pos, move))
		{
			change_side_to_move(pos, states);
			continue;
		}
		change_side_to_move(pos, states);

		if (!foundMove)
		{
			foundMove = true;
			cout << "yes, this move permits the following good moves:" << endl;
		}

		cout << "\t" << item.first << " after " << square_to_string(square);
		cout << " is undefended" << endl;
	}
}

void Test::log_is_move_discovered_attack()
{
	Move tokenMove = UCI::to_move(*pos, token);

	vector<ExtMove> discoveredAttacks = get_discovered_attacks_after_move(pos, states, tokenMove);

	if (discoveredAttacks.size() == 0)
	{
		cout << "no, move is not a discovered attack" << endl;
		return;
	}

	cout << "yes, move is a discovered attack through these follow-up moves:" << endl;

	for (ExtMove move : discoveredAttacks)
	{
		cout << "\t" << move_to_string(move) << endl;
	}
}

#pragma endregion
