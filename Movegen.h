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