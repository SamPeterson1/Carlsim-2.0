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

#ifndef TRANSPOSITION_TABLE_H

#define TRANSPOSITION_TABLE_H

#include "Log.h"
#include "Zobrist.h"
#include "Move.h"

#define TT_TABLE_SIZE (1ULL << 24)

#define ENTRY_EXACT 0
#define ENTRY_ALPHA 1
#define ENTRY_BETA 2

#define TT_LOOKUP_FAILED -2147483647

typedef struct TTEntry_s {
    ZobristKey key;
    Move bestMove;

    int eval;
    int depth;
    int type;
} TTEntry;

int tt_getEval(ZobristKey key, int alpha, int beta, int depth, Move *bestMove);
void tt_storeEval(ZobristKey key, int eval, int depth, int evalType, Move move);
void tt_storeMove(ZobristKey key, Move move, int depth);

#endif