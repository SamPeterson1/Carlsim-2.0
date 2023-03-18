#ifndef MOVEGEN_H

#define MOVEGEN_H

#include "Move.h"
#include "MagicHashTable.h"
#include "Board.h"
#include <sys/time.h>

#define MG_MAX_MOVES 256

#define WHITE_KINGSIDE_CASTLE_MASK 0x70
#define WHITE_QUEENSIDE_CASTLE_MASK 0x1C
#define BLACK_QUEENSIDE_CASTLE_MASK 0x1C00000000000000
#define BLACK_KINGSIDE_CASTLE_MASK 0x7000000000000000

#define NORTH 0
#define NORTHEAST 1
#define EAST 2
#define SOUTHEAST 3
#define SOUTH 4
#define SOUTHWEST 5
#define WEST 6
#define NORTHWEST 7
#define NONE 8

void mg_init(void);
int mg_gen(Board *board, Move *moves);

#endif