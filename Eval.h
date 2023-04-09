#ifndef EVAL_H

#define EVAL_H

#include "Move.h"
#include "Board.h"
#include "Movegen.h"

#define EVAL_INF 65535

#define PAWN_VALUE 71
#define KNIGHT_VALUE 293
#define BISHOP_VALUE 300
#define ROOK_VALUE 456
#define QUEEN_VALUE 905

/*template<turn: WHITE | BLACK>*/
int evalMove(Board *board, Move move);
/*endtemplate*/

/*template<turn: WHITE | BLACK>*/
int evalPosition(Board *board);
/*endtemplate*/


#endif