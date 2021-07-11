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

This repository mostly consists of Stockfish source code, which can be found here: https://github.com/official-stockfish/Stockfish. The only files that were edited/created outside of this repository are the following:
```
- uci.cpp 	: edited, added command <strong>test -arg -arg</strong>, which logs some of the test requirements above
- test.h 	: added file
- test.cpp	: added file
```
	
To run the test, compile the program into an executable and run it from the command line. Here, the program will execute identically to how Stockfish runs with the only addition that the command <strong>test -arg -arg</strong> can also be executed. For the <strong>test -arg -arg</strong> help menu, simply type <strong>test</strong>. Below are some examples.
```
- Piece Function Ideas 
	test piece e4
- Position Function Ideas 
	test position 10
- Position Function Ideas 
	test move e2e4
```
	
## Evaluation

#### Number of Objectives Achieved: 4 / 13

To be frank, this technical test was pretty rough for me. While <strong>I felt that the listed requirements/objectives were not that challenging</strong>, understanding the framework was very difficult. Before this examination, <strong>I had never worked with UCI protocols or any chess engine</strong> other than my own (which was not UCI compliant); as a result, the vast <strong>majority of time bottlenecks arose from reading and comprehending the API and source code</strong> for both chess engine and GUI. And wow! I could not find any sort of documentation explaining the code behind Stockfish other than the comments that were provided directly in the source!

Overall, a score of 4 / 13 is not ideal. However, I truly believe that had I previously been familiar with the source materials, I would have achieved a far higher score. Please take this into consideration.

## Final Notes

Thank you so much for the opportunity  and chance to work with Chess.com. This was incredibly exciting for me as the prospect of working with my favorite company and my favorite pastime was just awesome. I definitely feel much more prepared to work with chess engines in the future!