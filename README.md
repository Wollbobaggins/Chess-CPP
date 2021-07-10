## Objective

<strong>In C++, (or another language if necessary) make objects with functions that provide the kind of information that Chess players care about. You will probably be able to do a lot more if you use open-source code to accelerate your work, such as  https://github.com/official-stockfish/Stockfish or https://github.com/AndyGrant/Ethereal.  If you're already familiar with them, all the better!</strong>

## Requirements

Some ideas are suggested below. You won't be able to do all of these things in the time provided — that's OK — do what you can! 

### Piece function ideas
- [ ] how many legal moves does it have
- [ ] how many pieces can it capture
- [ ] is it pinned
- [ ] is it hanging
- [ ] how many legal moves are there for all the pieces it can capture

### Position function ideas
- [ ] get a vector of the best N moves from the position (using an engine)
- [ ] get a vector of threat moves from the position (moves that would be good for the player not-to-move)

### Move function ideas
- [ ] how many centipawns are lost by this move
- [ ] does this move capture a hanging piece
- [ ] is this move a winning capture according to Static Exchange Evaluation? (https://www.chessprogramming.org/Static_Exchange_Evaluation)
- [ ] does this move allow the opponent to make a winning capture according to Static Exchange Evaluation?
- [ ] does this move *permit* the opponent to play a good move?
- [ ] does this move *permit* the opponent to play a good move, by undefending a specific square?

### Additional ideas
- [ ] pending requirements...