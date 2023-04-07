/*
Carlsim 2.0 Chess Engine
Copyright (C) 2023 Sam Peterson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MOVEGEN_H

#define MOVEGEN_H

#include "Move.h"
#include "MagicHashTable.h"
#include "Board.h"
#include <sys/time.h>

#define MG_MAX_MOVES 256

#define WHITE_KINGSIDE_CASTLE_MASK 0x60ULL
#define WHITE_QUEENSIDE_CASTLE_MASK 0x0CULL
#define BLACK_QUEENSIDE_CASTLE_MASK 0xC00000000000000ULL
#define BLACK_KINGSIDE_CASTLE_MASK 0x6000000000000000ULL

#define CREATE_PROMOTIONS(moves, movec, origin, dest) Move move = CREATE_MOVE((origin), (dest), QUIET); moves[(movec) ++] = move | QUEEN_PROMOTION; moves[(movec) ++] = move | ROOK_PROMOTION; moves[(movec) ++] = move | BISHOP_PROMOTION; moves[(movec) ++] = move | KNIGHT_PROMOTION

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

int mg_gen_t39E5CDBB(Board *board, Move *moves);
int mg_gen_tF036FC27(Board *board, Move *moves);

#endif
