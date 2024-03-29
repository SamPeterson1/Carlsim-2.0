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

#ifndef MHT_H

#define MHT_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Board.h"

typedef struct MagicHashTable_s {
    int invBits;
    uint64_t magic;
    Bitboard *items;
} MagicHashTable;

#define MHT_GET_VALUE(t, k) ((t)->items[((k) * (t)->magic) >> ((t)->invBits)])

MagicHashTable *mht_create(int bits, uint64_t magic);
void mht_insert(MagicHashTable *table, Bitboard key, Bitboard value);

#endif