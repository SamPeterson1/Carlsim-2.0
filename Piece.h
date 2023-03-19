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

#ifndef PIECE_H

#define PIECE_H

#include <stdint.h>
#include <ctype.h>

#define WHITE 0
#define BLACK 1

#define PAWN 0
#define KNIGHT 2
#define BISHOP 4
#define ROOK 6
#define QUEEN 8
#define KING 10

#define PIECE_NONE 255

#define PIECE_TYPE(piece) ((piece) >> 1)
#define PIECE_COLOR(piece) ((piece) & 1)

typedef uint8_t Piece;

Piece pc_parse(char c);
char pc_toChar(Piece piece);

#endif