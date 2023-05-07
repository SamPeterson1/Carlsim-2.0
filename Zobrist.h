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

#ifndef ZOBRIST_H

#define ZOBRIST_H

#include "Board.h"
#include <stdlib.h>

#define Z_WHITE_KINGSIDE_CASTLE 0
#define Z_BLACK_KINGSIDE_CASTLE 1
#define Z_WHITE_QUEENSIDE_CASTLE 2
#define Z_BLACK_QUEENSIDE_CASTLE 3

extern ZobristKey g_zPieceSquareKeys[12][64];
extern ZobristKey g_zBlackToMoveKey;
extern ZobristKey g_zCastleKeys[4];
extern ZobristKey g_zCastleRightKeys[16];
extern ZobristKey g_zEpFileKeys[8];

void z_init(void);
ZobristKey z_getKey(Board *board);
int z_verify(Board *board);

#endif