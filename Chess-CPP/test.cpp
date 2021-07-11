#include "test.h"

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
	//if (token is not valid) return; // HACK: assumes token is valid

	log_piece_legal_moves();
	log_piece_available_captures();
	log_piece_is_pinned();
	log_piece_is_hanging();
}

void Test::log_piece_legal_moves()
{
	// HACK: only works for the piece color that is about to move
	MoveList list = MoveList<LEGAL>(*pos); 
	int legal_move_count = 0;

	// HACK: rough around the edges, but if program had a compatible GUI or general chess program, 
	// this would obviously be optimized rather than using string comparisons for getting moves
	for (ExtMove m : list)
	{
		if (UCI::square(from_sq(m)) == token)  legal_move_count++;
	}

	cout << "piece on " << token << " has " << legal_move_count << " legal moves:" << endl;
	for (ExtMove m : list) log_move_if_valid(m);
}

void Test::log_piece_available_captures()
{
	// HACK: only works for the piece color that is about to move
	MoveList list = MoveList<CAPTURES>(*pos);
	int legal_move_count = 0;

	// HACK: rough around the edges, but if program had a compatible GUI or general chess program, 
	// this would obviously be optimized rather than using string comparisons for getting moves
	for (ExtMove m : list)
	{
		if (UCI::square(from_sq(m)) == token) legal_move_count++;
	}

	cout << "piece on " << token << " has " << legal_move_count << " captures:" << endl;
	for (ExtMove m : list) log_move_if_valid(m);
}

void Test::log_piece_is_pinned()
{
	//Bitboard occupied = pieces() ^ from ^ to;
	Square square = string_to_square(token);

	Color color = square_to_piece_color(square);

	Bitboard block_board = pos->blockers_for_king(color);

	bool is_pinned = block_board & square;

	cout << "is piece absolute pinned? ";
	cout << (is_pinned ? "yes" : "no") << endl;
}

void Test::log_piece_is_hanging()
{
	Square square = string_to_square(token);

	Color color = square_to_piece_color(square);

	Bitboard ally_piece_board = pos->pieces(color);
	Bitboard enemy_piece_board = pos->pieces(~color);
	Bitboard attack_board = pos->attackers_to(square);

	bool is_defended = ally_piece_board & attack_board;
	bool is_attacked = enemy_piece_board & attack_board;

	bool is_hanging = !is_defended && is_attacked;

	cout << "is piece hanging? ";
	cout << (is_hanging ? "yes" : "no") << endl;
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

#pragma region Utility Code

void Test::log_move_if_valid(ExtMove m)
{
	if (UCI::square(from_sq(m)) == token)
		cout << "\t" << UCI::square(from_sq(m)) << UCI::square(to_sq(m)) << endl;
}

Square Test::string_to_square(string& str)
{
	if (str.size() != 2) return SQ_NONE;

	string alphabet = "abcdefgh";
	string numbers = "12345678";

	File f = static_cast<File>(alphabet.find(str.at(0)));
	Rank r = static_cast<Rank>(numbers.find(str.at(1)));

	return make_square(f, r);
}

Color Test::square_to_piece_color(Square square)
{
	Piece piece = pos->piece_on(square);

	if (piece == NO_PIECE) return COLOR_NB;

	return color_of(piece);
}

#pragma endregion

#pragma region Debug Functions

void Test::debug_log_bitboard(Bitboard b) 
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
