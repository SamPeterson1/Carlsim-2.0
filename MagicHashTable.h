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