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

#include "Piece.h"
#include <stdio.h>

Piece pc_parse(char c) {
    int type = PIECE_NONE;

    switch(tolower(c)) {
        case 'p':
            type = PAWN;
            break;
        case 'n':
            type = KNIGHT;
            break;
        case 'b':
            type = BISHOP;
            break;
        case 'r':
            type = ROOK;
            break;
        case 'q':
            type = QUEEN;
            break;
        case 'k':
            type = KING;
            break;
    }

    printf("Char: %c, type: %d\n", c, type);

    int color = (c < 97) ? WHITE : BLACK;

    return color | type;
}

char pc_toChar(Piece piece) {
    if(piece == PIECE_NONE) return '-';
    char c = ' ';

    switch(PIECE_TYPE(piece)) {
        case PAWN:
            c = 'p';
            break;
        case KNIGHT:
            c = 'n';
            break;
        case BISHOP:
            c = 'b';
            break;
        case ROOK:
            c = 'r';
            break;
        case QUEEN:
            c = 'q';
            break;
        case KING:
            c = 'k';
            break;
    }

    if(PIECE_COLOR(piece) == WHITE) c -= 32;

    return c;
}
