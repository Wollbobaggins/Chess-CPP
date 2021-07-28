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
- [x] (6) does this move *permit* the opponent to play a good move, by undefending a specific square?
- [x] (+) does this move create discovered attacks?

## Execution

This repository mostly consists of Stockfish source code, which can be found here: https://github.com/official-stockfish/Stockfish. The only files that were edited/created outside of this repository are the following:

```- uci.cpp``` : edited on line 238 & line 291, added command line function "test -args"
```- movegen.cpp```	: edited on line 238, removed assertion when color is in check
```- test.h``` : added file
```- test.cpp``` : added file
```- utilities.h```	: added file
	
To run the test, compile the program into an executable and run it from the command line. Here, the program will execute identically to how Stockfish runs with the only addition that the command <strong>test -args</strong> can also be executed. For the <strong>test -args</strong> help menu, simply type <strong>test</strong>. Below are some examples that have been run from the default start position.

#### Help Menu
```test```
```
here is a list of available commands for test:
	test settings -args : set the following options...
		depth -arg : how deep for the engine to search
		see -arg : threshold for winning static exchange evaluation
		permit -arg : centipawn permit difference to be classified as good move
		reset : restores settings to defaults
	test piece -arg : a square in FILE/RANK form
	test position -arg : number of moves to display
	test move -arg : a move in FILE/RANK/FILE/RANK form

current settings:
	depth: 8
	static exchange threshold: 126
	centipawn permit difference: 200
```
#### Function Settings
```test settings depth 5 permit 300```
```
current settings:
	depth: 5
	static exchange threshold: 126
	centipawn permit difference: 300
```
#### Piece Function Ideas 
```test piece e2```
```
(1) piece on e2 has 2 legal moves:
	e2e3
	e2e4
(2) piece on e2 has 0 captures:
(3) is piece absolute pinned? no
(4) is piece hanging? no
(5) piece cannot capture, no capturable pieces found
```
#### Position Function Ideas 
```test position 3```
```
(1) logging best 3 moves
	h2h3: 90 centipawns
	b1c3: 65 centipawns
	c2c4: 35 centipawns
	no more moves found
(2) logging threat 3 moves
	g2g4: -162 centipawns
	f2f3: -98 centipawns
	g1h3: -70 centipawns
	no more moves found
```
#### Position Function Ideas 
```test move e2e4```
```
(1) centipawn loss of 35
(2) no, does not capture a hanging piece on e4
(3) no, Static Exchange Evaluation does not apply, move is not a capture
(4) no, move does not allow the opponent to make a winning capture according to Static Exchange Evaluation
(5) no, this move does not permit a good move
(6) no, this move does not permit a good move by undefending a specific square
(+) no, move is not a discovered attack
```
## Evaluation

#### Number of Objectives Achieved: 14 / 13

we'll rewrite this soon...

## Final Notes

Again, thank you so much for the opportunity and chance to work with Chess.com. This was incredibly exciting for me as the prospect of working with my favorite company and my favorite pastime was just awesome. I definitely feel much more prepared to work with chess engines in the future!