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

#include "MagicHashTable.h"

MagicHashTable *mht_create(int bits, uint64_t magic) {
    MagicHashTable *table = (MagicHashTable *) malloc(sizeof(MagicHashTable));
    table->invBits = 64 - bits;
    table->magic = magic;
    int size = 1 << bits;
    table->items = (Bitboard*) calloc(size, sizeof(Bitboard));
    for (int i=0; i<size; i++)
        table->items[i] = 0;
    return table;
}

void mht_insert(MagicHashTable *table, Bitboard key, Bitboard value) {
    uint16_t hash = (key * table->magic) >> (uint64_t)(table->invBits);
    table->items[hash] = value;
}