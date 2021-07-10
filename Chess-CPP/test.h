#pragma once

//#include <cassert>
//#include <cmath>
#include <iostream>
//#include <sstream>
//#include <string>
//
//#include "evaluate.h"
//#include "movegen.h"
#include "Stockfish src/position.h"
//#include "search.h"
//#include "thread.h"
//#include "timeman.h"
//#include "tt.h"
//#include "uci.h"
//#include "syzygy/tbprobe.h"

using namespace std;
using namespace Stockfish;

class Test
{
public:
	Test(Position& pos, istringstream& is, StateListPtr& states);

private:
	void HelloWorld();
};

