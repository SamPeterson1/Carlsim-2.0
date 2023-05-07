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

#include "Zobrist.h"

ZobristKey g_zPieceSquareKeys[12][64];
ZobristKey g_zBlackToMoveKey;
ZobristKey g_zCastleKeys[4];
ZobristKey g_zCastleRightKeys[16];
ZobristKey g_zEpFileKeys[8];

ZobristKey lrand(void) {
    return (((ZobristKey) rand()) >> 32ULL) | ((ZobristKey) rand());
}

void z_init(void) {
    for (int piece = 0; piece < 12; piece ++) {
        for (int square = 0; square < 64; square ++) {
            g_zPieceSquareKeys[piece][square] = lrand();
        }
    }

    g_zBlackToMoveKey = lrand();

    for (int i = 0; i < 16; i ++)
        g_zCastleRightKeys[i] = lrand();

    g_zCastleKeys[Z_WHITE_KINGSIDE_CASTLE] = g_zPieceSquareKeys[WHITE_KING][6] ^ g_zPieceSquareKeys[WHITE_KING][4];
    g_zCastleKeys[Z_WHITE_KINGSIDE_CASTLE] ^= g_zPieceSquareKeys[WHITE_ROOK][5] ^ g_zPieceSquareKeys[WHITE_ROOK][7];

    g_zCastleKeys[Z_BLACK_KINGSIDE_CASTLE] = g_zPieceSquareKeys[BLACK_KING][62] ^ g_zPieceSquareKeys[BLACK_KING][60];
    g_zCastleKeys[Z_BLACK_KINGSIDE_CASTLE] ^= g_zPieceSquareKeys[BLACK_ROOK][61] ^ g_zPieceSquareKeys[BLACK_ROOK][63];
    
    g_zCastleKeys[Z_WHITE_QUEENSIDE_CASTLE] = g_zPieceSquareKeys[WHITE_KING][2] ^ g_zPieceSquareKeys[WHITE_KING][4];
    g_zCastleKeys[Z_WHITE_QUEENSIDE_CASTLE] ^= g_zPieceSquareKeys[WHITE_ROOK][3] ^ g_zPieceSquareKeys[WHITE_ROOK][0];

    g_zCastleKeys[Z_BLACK_QUEENSIDE_CASTLE] = g_zPieceSquareKeys[BLACK_KING][58] ^ g_zPieceSquareKeys[BLACK_KING][60];
    g_zCastleKeys[Z_BLACK_QUEENSIDE_CASTLE] ^= g_zPieceSquareKeys[BLACK_ROOK][59] ^ g_zPieceSquareKeys[BLACK_ROOK][56];

    for (int i = 0; i < 8; i ++)
        g_zEpFileKeys[i] = lrand();
}

ZobristKey z_getKey(Board *board) {
    ZobristKey zobrist = 0;

    for (int square = 0; square < 64; square ++) {
        Piece piece = board->pieces[square];

        if (piece != PIECE_NONE)
            zobrist ^= g_zPieceSquareKeys[piece][square];
    }

    int epFile = EP_FILE(board);
    if (epFile != -1) 
        zobrist ^= g_zEpFileKeys[epFile];

    zobrist ^= g_zCastleRightKeys[GET_CASTLE_RIGHTS(board)];
    
    if (TURN(board) == BLACK)
        zobrist ^= g_zBlackToMoveKey;
    
    return zobrist;
}

int z_verify(Board *board) {
    return (board->zobrist == z_getKey(board));
}

