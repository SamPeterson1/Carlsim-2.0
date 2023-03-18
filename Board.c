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

#include "Board.h"

void bd_print(Board *board) {
    printf("\e[1;1H\e[2J");

    printf("White kingside castle right: %d\n", HAS_CASTLE_RIGHT(board, WHITE_CASTLE_KINGSIDE_RIGHT));
    printf("White queenside castle right: %d\n", HAS_CASTLE_RIGHT(board, WHITE_CASTLE_QUEENSIDE_RIGHT));
    printf("Black kingside castle right: %d\n", HAS_CASTLE_RIGHT(board, BLACK_CASTLE_KINGSIDE_RIGHT));
    printf("Black queenside castle right: %d\n", HAS_CASTLE_RIGHT(board, BLACK_CASTLE_QUEENSIDE_RIGHT));

    for( int rank = 7; rank >= 0; rank --) {
        printf("    +---+---+---+---+---+---+---+---+\n");
        printf("(%d) ", rank+1);
        for (int file = 0; file < 8; file ++) {
            printf("| %c ", pc_toChar(board->pieces[SQUARE(rank, file)]));
        }
        printf("|\n");
    }

    printf("    +---+---+---+---+---+---+---+---+\n");
    printf("     (a) (b) (c) (d) (e) (f) (g) (h)\n\n");
}

void bd_clear(Board *board) {
    for (int i = 0; i < 12; i ++)
        PIECE_BITBOARD(board, i) = 0;
    
    for (int i = 0; i < 64; i ++)
        board->pieces[i] = PIECE_NONE;

    board->gameState = 0;
    board->bitboard = 0;
    board->colorBitboards[WHITE] = 0;
    board->colorBitboards[BLACK] = 0;
}

void bd_movePiece(Board *board, int origin, int dest) {
    Piece piece = board->pieces[origin];
    Bitboard move = (1ULL << origin) | (1ULL << dest);

    board->bitboard ^= move;
    PIECE_BITBOARD(board, piece) ^= move;
    COLOR_BITBOARD(board, piece) ^= move;

    board->pieces[origin] = PIECE_NONE;
    board->pieces[dest] = piece;
}


void bd_addPiece(Board *board, Piece piece, int square) {
    Bitboard position = (1ULL << square);

    board->bitboard |= position;
    PIECE_BITBOARD(board, piece) |= position;
    COLOR_BITBOARD(board, piece) |= position;

    board->pieces[square] = piece;
}

void bd_clearPiece(Board *board, int square) {
    Piece piece = board->pieces[square];
    if (piece == PIECE_NONE) return;

    Bitboard mask = ~(1ULL << square);

    board->bitboard &= mask;
    PIECE_BITBOARD(board, piece) &= mask;
    COLOR_BITBOARD(board, piece) &= mask;

    board->pieces[square] = PIECE_NONE;
}

void bd_squareToStr(int square, char *str) {
    int rank = RANK(square);
    int file = FILE(square);

    str[0] = FILE_TO_CHAR(file);
    str[1] = RANK_TO_CHAR(rank);
    str[2] = '\0';
}