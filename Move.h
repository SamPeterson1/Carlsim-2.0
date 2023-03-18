#ifndef MOVE_H

#define MOVE_H
#include <stdint.h>
#include "Board.h"

typedef uint16_t Move;

#define LAN_MAX_SIZE 6

#define QUIET 0
#define DOUBLE_PAWN_PUSH 0x1000

#define KINGSIDE_CASTLE 0x2000
#define QUEENSIDE_CASTLE 0x3000

#define CAPTURE 0x4000
#define EP_CAPTURE 0x5000

#define PROMOTION 0x8000
#define KNIGHT_PROMOTION 0x8000
#define BISHOP_PROMOTION 0x9000
#define ROOK_PROMOTION 0xA000
#define QUEEN_PROMOTION 0xB000

#define CREATE_MOVE(origin, dest, special) ((origin) | ((dest) << 6) | (special))

#define MOVE_ORIGIN(move) ((move) & 0x3F)
#define MOVE_DEST(move) (((move) & 0xFC0) >> 6)
#define MOVE_IS_CAPTURE(move) (((move) >> 14) & 1)
#define MOVE_IS_PROMOTION(move) (((move) >> 15) & 1)
#define MOVE_SPECIAL(move) ((move) & 0xF000)

#define MOVE_RESULT(m, board) {.prevGameState = board->gameState, .captured = board->pieces[MOVE_DEST(m)], .move = (m)}

typedef struct MoveResult_s {
    uint16_t prevGameState;
    Piece captured;
    Move move;
} MoveResult;

Move mv_fromLAN(Board *board, char *str);
void mv_toLAN(Move move, char *str);
void mv_make(Move move, Board *board);
void mv_unmake(MoveResult *result, Board *board);

#endif