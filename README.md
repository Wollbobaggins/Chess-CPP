# Requirements

## Piece function ideas
[_] how many legal moves does it have
** how many pieces can it capture
** is it pinned
** is it hanging
** how many legal moves are there for all the pieces it can capture

* Position function ideas
** get a vector of the best N moves from the position (using an engine)
** get a vector of threat moves from the position (moves that would be good for the player not-to-move)

* Move function ideas
** how many centipawns are lost by this move
** does this move capture a hanging piece
** is this move a winning capture according to Static Exchange Evaluation? (https://www.chessprogramming.org/Static_Exchange_Evaluation)
** does this move allow the opponent to make a winning capture according to Static Exchange Evaluation?
** does this move *permit* the opponent to play a good move?
** does this move *permit* the opponent to play a good move, by undefending a specific square?