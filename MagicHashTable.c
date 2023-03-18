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