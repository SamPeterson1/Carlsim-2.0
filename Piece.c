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

    int color = (c < 97) ? WHITE : BLACK;

    return CREATE_PIECE(color, type);
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
