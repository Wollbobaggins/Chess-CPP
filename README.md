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
- [x] (5) does this move *permit* the opponent to play a good move?
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

#### Number of Objectives Achieved: 12 / 13

Shoot, so close! I was super close to finishing all the tasks, but I certainly underestimated how long the last two objectives would take. Again, I feel that my progress was not ideal and if only I had another day to look a the problems that popped up, finishing this test would have been doable. Unlike last time, I was unable to work the entire allotted period. My family has been in the process of moving this week and squeezing time to work on the project before Friday was a little challenging. Hopefully, in the future, I will be much more prepared to work with UCI Chess engines as I certainly felt more familiar with the application the second time around working with it.

While all tasks work as intended, the last two tasks were not up to par. I couldn't figure out how to set the position of Stockfish's internal Bitboard, and then roll back to a previous position without having complications. As a result, the final task I was unable to complete due to the issues that surrounded the aforementioned problem. In my source code, I wrote out the intended solution via pseudocode, which was the planned implementation of the final task. I still firmly believe that the concepts in programming this test are achievable given a little more source knowledge.

## Final Notes

Again, thank you so much for the opportunity and chance to work with Chess.com. This was incredibly exciting for me as the prospect of working with my favorite company and my favorite pastime was just awesome. I definitely feel much more prepared to work with chess engines in the future!