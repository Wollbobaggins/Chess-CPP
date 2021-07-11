## Objective

<strong>In C++, (or another language if necessary) make objects with functions that provide the kind of information that Chess players care about. You will probably be able to do a lot more if you use open-source code to accelerate your work, such as  https://github.com/official-stockfish/Stockfish or https://github.com/AndyGrant/Ethereal.  If you're already familiar with them, all the better!</strong>

## Requirements

Some ideas are suggested below. You won't be able to do all of these things in the time provided — that's OK — do what you can! 

#### Piece function ideas
- [x] how many legal moves does it have
- [x] how many pieces can it capture
- [x] is it pinned
- [x] is it hanging
- [ ] how many legal moves are there for all the pieces it can capture

#### Position function ideas
- [ ] get a vector of the best N moves from the position (using an engine)
- [ ] get a vector of threat moves from the position (moves that would be good for the player not-to-move)

#### Move function ideas
- [ ] how many centipawns are lost by this move
- [ ] does this move capture a hanging piece
- [ ] is this move a winning capture according to Static Exchange Evaluation? (https://www.chessprogramming.org/Static_Exchange_Evaluation)
- [ ] does this move allow the opponent to make a winning capture according to Static Exchange Evaluation?
- [ ] does this move *permit* the opponent to play a good move?
- [ ] does this move *permit* the opponent to play a good move, by undefending a specific square?

## Execution

This repository mostly consists of Stockfish source code, which can be found here: https://github.com/official-stockfish/Stockfish. The only files that were edtited/created outside of this repository are the following:
		- uci.cpp : added command <strong>test -arg -arg</strong>, which logs some of the test requirements above
		- test.h
		- test.cpp
	
To run the test, compile the program into an executable and run it from the command line. Here, the program will execute identical to how Stockfish runs with the only addition that the command <strong>test -arg -arg</strong> can also be executed. For the <strong>test -arg -arg</strong> help menu, simply type <strong>test</strong>. Below are some examples.
		- Position function ideas 
		- Position function ideas 
		- Position function ideas 
	
## Evaluation

Honestly, this technical test was pretty rough for me. While I felt that the listed requirements/objectives were not that challenging, understanding the framework that I attempted to complete the test in was very difficult. Previously before this examination, I had never worked with UCI protocols or any chess engine other than my own (which was not UCI compliant) as a result, the vast majority of time bottlenecks were a result of reading and comprehending the API for both chess engine and GUI. And wow! I could not find any sort of documentation explaining the code behind Stockfish other than the comments that were provided directly in the source!

