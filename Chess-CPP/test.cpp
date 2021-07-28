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
		if (token == "settings") set_settings();
		else if (token == "piece") run_piece_function_ideas();
		else if (token == "position") run_position_function_ideas();
		else if (token == "move") run_move_function_ideas();
		else if (token == "tests") run_tests();
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
	token = "";

	*is >> token;

	return !token.empty();
}

void Test::log_help()
{
	cout << "here is a list of available commands for test:" << endl;

	cout << "\ttest settings -args : set the following options..." << endl;
	cout << "\t\tdepth -arg : how deep for the engine to search" << endl;
	cout << "\t\tsee -arg : threshold for winning static exchange evaluation" << endl;
	cout << "\t\tpermit -arg : centipawn permit difference to be classified as good move" << endl;
	cout << "\t\treset : restores settings to defaults" << endl;
	cout << "\ttest piece -arg : a square in FILE/RANK form" << endl;
	cout << "\ttest position -arg : number of moves to display" << endl;
	cout << "\ttest move -arg : a move in FILE/RANK/FILE/RANK form" << endl;
	cout << "\ttest tests : runs four tests outputting the functionality of this program" << endl;
	cout << endl;

	log_settings();
}

void Test::set_settings()
{
	while (read_next_token())
	{
		string str = token;
		read_next_token();

		if (str == "depth") depth = stoi(token);
		else if (str == "see") threshold = Value(stoi(token));
		else if (str == "permit") permitCentipawnLoss = stoi(token);
		else if (str == "reset")
		{
			depth = 5;
			threshold = PawnValueMg;
			permitCentipawnLoss = 200;
		}
		else cout << "unrecognized argument: " << token << ", please check help menu" << endl;
	} 

	log_settings();
}

void Test::log_settings()
{
	cout << "current settings:" << endl;

	cout << "\tdepth: " << depth << endl;
	cout << "\tstatic exchange threshold: " << threshold << endl;
	cout << "\tcentipawn permit difference: " << permitCentipawnLoss << endl;
	cout << endl;
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

	cout << endl;
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

	bool foundMove = false;

	MoveList legalMoves = MoveList<LEGAL>(*pos);
	MoveList captures = MoveList<CAPTURES>(*pos);

	change_side_to_move(pos, states);

	for (ExtMove move : captures)
	{
		if (from_sq(move) == square && legalMoves.contains(move))
		{
			if (!foundMove)
			{
				cout << "logging legal moves for capturable pieces:" << endl; 
				foundMove = true;
			}

			token = square_to_string(to_sq(move)); // we can edit token, last time it's used
			log_piece_legal_moves();
		}
	}

	change_side_to_move(pos, states);

	if (!foundMove) cout << "piece cannot capture, no capturable pieces found" << endl;
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

	cout << endl;
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

	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states, depth);

	for (pair<string, int> item : moveEvaluations)
	{
		if (n == 0) break;

		cout << "\t" << item.first << ": " << int_to_string_evaluation(item.second) << endl;

		n--;
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

	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states, depth);

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

	cout << endl;
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
	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states, depth);
	int loss = get_centipawn_loss(moveEvaluations, token);

	if (loss == 0) cout << "no centipawn loss, bestmove found by engine" << endl;
	else if (abs(loss) >= VALUE_INFINITE)  cout << "massive centipawn loss, missed mate!" << endl;
	else cout << "centipawn loss of " << loss << endl;
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

	Move tokenMove = UCI::to_move(*pos, token);
	make_move(pos, states, tokenMove);

	MoveList legalMoves = MoveList<LEGAL>(*pos);
	MoveList captures = MoveList<CAPTURES>(*pos);

	bool foundMove = false;

	for (ExtMove move : captures)
	{
		if (legalMoves.contains(move))
		{
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
	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states, depth);

	if (get_centipawn_loss(moveEvaluations, token) < permitCentipawnLoss)
	{
		cout << "no, this move does not permit a good move" << endl;
		return;
	}

	bool foundMove = false;

	Move tokenMove = UCI::to_move(*pos, token);

	vector<pair<string, int>> permittedGoodMoves = get_permitted_good_moves(
		pos, states, depth, moveEvaluations, permitCentipawnLoss, tokenMove);

	for (pair<string, int> item : permittedGoodMoves)
	{
		if (!foundMove)
		{
			foundMove = true;
			cout << "yes, this move permits the following good moves: " << endl;
		}

		cout << "\t" << item.first << ": " << int_to_string_evaluation(-item.second) << endl;
	}

	if (!foundMove) cout << "no, this move does not permit a good move" << endl;
}

void Test::log_does_permit_good_move_by_undefending_square()
{
	bool foundMove = false;

	vector<pair<string, int>> moveEvaluations = get_move_evaluations(pos, states, depth);

	if (get_centipawn_loss(moveEvaluations, token) < permitCentipawnLoss)
	{
		cout << "no, this move does not permit a good move ";
		cout << "by undefending a specific square" << endl;
		return;
	}

	Color color = pos->side_to_move();
	Move tokenMove = UCI::to_move(*pos, token);

	vector<pair<string, int>> permittedGoodMoves = get_permitted_good_moves(
		pos, states, depth, moveEvaluations, permitCentipawnLoss, tokenMove);

	for (pair<string, int> item : permittedGoodMoves)
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

	if (!foundMove)
	{
		cout << "no, this move does not permit a good move ";
		cout << "by undefending a specific square" << endl;
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

#pragma region Test Functions

void Test::run_all_functions(string& pieceToken, string& positionToken, string& moveToken)
{
	cout << "Executing Piece Function Test" << endl;
	token = pieceToken;

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

	cout << endl;

	cout << "Executing Position Function Test" << endl;
	token = positionToken;

	cout << "(1) "; // Position Function Idea 1
	log_best_moves();

	cout << "(2) "; // Position Function Idea 2
	log_threat_moves();

	cout << endl;

	cout << "Executing Move Function Test" << endl;
	token = moveToken;

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

	cout << endl;
}

void Test::run_tests()
{
	test_double_scholars_mate_position();

	test_pins_and_discovered_attack();

	test_defended_with_undefending();

	test_undefended_without_undefending();
}

void Test::test_double_scholars_mate_position()
{
	string fen = "rnb2knr/pppp1ppp/8/2b1p2Q/2B1P2q/8/PPPP1PPP/RNB2KNR w KQkq - 0 1";

	set_position(pos, states, fen);

	cout << *pos << endl;

	string pieceToken = "h5";
	string positionToken = "5";
	string moveToken = "h5g4"; 

	run_all_functions(pieceToken, positionToken, moveToken);
}

void Test::test_pins_and_discovered_attack()
{
	string fen = "k2q4/pp6/8/1r1B2p1/3Q4/8/PP6/1KB2r2 w - - 0 1";

	set_position(pos, states, fen);

	cout << *pos << endl;

	string pieceToken = "c1";
	string positionToken = "5";
	string moveToken = "d5b7"; 

	run_all_functions(pieceToken, positionToken, moveToken);
}

void Test::test_defended_with_undefending()
{
	string fen = "6R1/8/5b2/7k/3q4/8/PP6/K1Q5 w - - 0 1";

	set_position(pos, states, fen);

	cout << *pos << endl;

	string pieceToken = "b2";
	string positionToken = "5";
	string moveToken = "c1f1"; 

	run_all_functions(pieceToken, positionToken, moveToken);
}

void Test::test_undefended_without_undefending()
{
	string fen = "k4r2/pp3p2/1q4p1/3N3p/5Q2/8/PP6/KB6 b - - 0 1";

	set_position(pos, states, fen);

	cout << *pos << endl;

	string pieceToken = "a1";
	string positionToken = "5";
	string moveToken = "b6g1"; 

	run_all_functions(pieceToken, positionToken, moveToken);
}

#pragma endregion
