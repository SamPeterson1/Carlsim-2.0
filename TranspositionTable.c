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

#include "TranspositionTable.h"

TTEntry table[TT_TABLE_SIZE];

int tt_getEval(ZobristKey key, int alpha, int beta, int depth, Move *bestMove) {
    TTEntry *ent = &table[key % TT_TABLE_SIZE];
    if(key == ent->key && ent->depth >= depth) {
        *bestMove = ent->bestMove;

        if(ent->type == ENTRY_EXACT) {
            return ent->eval;
        } else if(ent->type == ENTRY_BETA && ent->eval >= beta) {
            return ent->eval;
        } else if(ent->type == ENTRY_ALPHA && ent->eval <= alpha) {
            return ent->eval;
        }
    }

    return TT_LOOKUP_FAILED;
}

void tt_storeEval(ZobristKey key, int eval, int depth, int evalType, Move move) {
    TTEntry *ent = &table[key % TT_TABLE_SIZE];
    ent->type = evalType;
    ent->bestMove = move;
    ent->depth = depth;
    ent->eval = eval;
    ent->key = key;
}