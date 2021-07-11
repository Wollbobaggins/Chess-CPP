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

	cout << "\ttest piece (arg) - a single square" << endl;
	cout << "\ttest position" << endl;
	cout << "\ttest move (arg) - a move" << endl;
}

#pragma endregion

#pragma region Piece Functions Ideas

void Test::run_piece_function_ideas()
{
	read_next_token();
	//if (token is not valid) return;

	log_piece_legal_moves();
}

void Test::log_piece_legal_moves()
{
	MoveList list = MoveList<LEGAL>(*pos);
	int legal_move_count = 0;

	// this is rough around the edges, but if program had a compatible GUI or general chess program, 
	// this would obviously be optimized rather than using string comparisons for getting moves
	for (ExtMove m : list)
	{
		if (UCI::square(from_sq(m)) == token) 
			legal_move_count++;
	}

	cout << "piece on " << token << " has " << legal_move_count << " legal moves:" << endl;

	for (ExtMove m : list)
	{
		if (UCI::square(from_sq(m)) == token) 
			cout << "\t" << UCI::square(from_sq(m)) << UCI::square(to_sq(m)) << endl;
	}
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
//istream ReadToken(istringstream& is, string& token)
//{
//	return (is >> token);
//}

#pragma endregion