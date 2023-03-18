#ifndef PIECE_H

#define PIECE_H

#include <stdint.h>
#include <ctype.h>

#define WHITE 0
#define BLACK 1

#define PAWN 0
#define KNIGHT 1
#define BISHOP 2
#define ROOK 3
#define QUEEN 4
#define KING 5

#define PIECE_NONE 255

#define CREATE_PIECE(color, type) ((color) | (type << 1))
#define PIECE_TYPE(piece) ((piece) >> 1)
#define PIECE_COLOR(piece) ((piece) & 1)

typedef uint8_t Piece;

Piece pc_parse(char c);
char pc_toChar(Piece piece);

#endif