## Objective

<strong>In C++, (or another language if necessary) make objects with functions that provide the kind of information that Chess players care about. You will probably be able to do a lot more if you use open-source code to accelerate your work, such as  https://github.com/official-stockfish/Stockfish or https://github.com/AndyGrant/Ethereal.  If you're already familiar with them, all the better!</strong>

## Requirements

Some ideas are suggested below. You won't be able to do all of these things in the time provided — that's OK — do what you can! 

#### Piece function ideas
- [x] (1) how many legal moves does it have
- [x] (2) how many pieces can it capture
- [x] (3) is it pinned
- [x] (4) is it hanging
- [x] (5) how many legal moves are there for all the pieces it can capture

#### Position function ideas
- [x] (1) get a vector of the best N moves from the position (using an engine)
- [x] (2) get a vector of threat moves from the position (moves that would be good for the player not-to-move)

#### Move function ideas
- [x] (1) how many centipawns are lost by this move
- [x] (2) does this move capture a hanging piece
- [x] (3) is this move a winning capture according to Static Exchange Evaluation? (https://www.chessprogramming.org/Static_Exchange_Evaluation)
- [x] (4) does this move allow the opponent to make a winning capture according to Static Exchange Evaluation?
- [ ] (5) does this move *permit* the opponent to play a good move?
- [ ] (6) does this move *permit* the opponent to play a good move, by undefending a specific square?

## Execution

This repository mostly consists of Stockfish source code, which can be found here: https://github.com/official-stockfish/Stockfish. The only files that were edited/created outside of this repository are the following:
```
- uci.cpp		: edited on line 238 & line 291, added command line function "test -arg -arg"
- movegen.cpp	: edited on line 238, removed assertion when color is in check
- test.h 		: added file
- test.cpp		: added file
- utilities.h	: added file
```
	
To run the test, compile the program into an executable and run it from the command line. Here, the program will execute identically to how Stockfish runs with the only addition that the command <strong>test -arg -arg</strong> can also be executed. For the <strong>test -arg -arg</strong> help menu, simply type <strong>test</strong>. Below are some examples.

#### Help Menu
```test```
#### Piece Function Ideas 
```test piece e4```
#### Position Function Ideas 
```test position 10```
#### Position Function Ideas 
```test move e2e4```
	
## Evaluation

#### Number of Objectives Achieved: X / 13

We'll get to this description later...

## Final Notes

We'll get to this description later...