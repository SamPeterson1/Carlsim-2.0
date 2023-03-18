#ifndef FEN_H

#define FEN_H

#include <string.h>
#include <stdlib.h>

#include "Board.h"
#include "Piece.h"

#define STARTING_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

void fen_load(Board *board, char *fen);
void fen_get(Board *board, char *fen);

#endif