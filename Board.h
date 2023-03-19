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

#ifndef BOARD_H

#define BOARD_H

#include <stdint.h>
#include <stdio.h>
#include "Piece.h"

typedef uint_fast64_t Bitboard;

typedef struct Board_s {
    // Bits 0-3 store white and black kingside/queenside castling legality
    // Bits 4-7 file of ep square (starting at 1, so 0 = no ep square)
    // Bit 8 is turn
    // Bits 9-15 is halfmove counter
    uint16_t gameState;

    Piece pieces[64];

    Bitboard pieceBitboards[12];
    Bitboard bitboard;
    Bitboard colorBitboards[2];
} Board;

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define WHITE 0
#define BLACK 1

#define WHITE_CASTLE_QUEENSIDE_RIGHT 0x01
#define WHITE_CASTLE_KINGSIDE_RIGHT 0x02
#define BLACK_CASTLE_QUEENSIDE_RIGHT 0x04
#define BLACK_CASTLE_KINGSIDE_RIGHT 0x08

#define TURN(board) (((board)->gameState >> 8) & 1)

#define PAWNS(board, turn) (board)->pieceBitboards[PAWN | turn]
#define KNIGHTS(board, turn) (board)->pieceBitboards[KNIGHT | turn]
#define BISHOPS(board, turn) (board)->pieceBitboards[BISHOP | turn]
#define ROOKS(board, turn) (board)->pieceBitboards[ROOK | turn]
#define QUEENS(board, turn) (board)->pieceBitboards[QUEEN | turn]
#define KINGS(board, turn) (board)->pieceBitboards[KING | turn]

#define PIECE_BITBOARD(board, piece) board->pieceBitboards[piece]
#define COLOR_BITBOARD(board, piece) board->colorBitboards[PIECE_COLOR(piece)]

#define SET_TURN(board, turn) (board)->gameState = (((board)->gameState & 0xFEFF) | (turn << 8))
#define HAS_CASTLE_RIGHT(board, castle) (((board)->gameState & castle) == castle)
#define EP_FILE(board) ((((board)->gameState >> 4) & 0x0F) - 1)
#define HALFMOVE_COUNTER(board) ((board)->gameState >> 9)

#define SET_HALFMOVE_COUNTER(board, counter) ((board)->gameState = ((board)->gameState & 0x1FF) | ((counter) << 9))
#define SET_EP_FILE(board, epFile) (board)->gameState = ((board)->gameState & 0xFF0F) | (((epFile + 1) << 4))
#define REMOVE_CASTLE_RIGHT(board, castle) (board)->gameState &= ~(castle)
#define GIVE_CASTLE_RIGHT(board, castle) (board)->gameState |= (castle)
#define CHANGE_TURN(board) ((board)->gameState ^= 0x100)

#define SET_PIECE(board, piece, position) PIECE_BITBOARD(board, piece) |= (1ULL << (position));
#define CLEAR_PIECE(board, piece, position) PIECE_BITBOARD(board, piece) &= ~(1ULL << (position));

#define RANK(square) ((square) / 8)
#define FILE(square) ((square) % 8)
#define SQUARE(rank, file) ((rank) * 8 + (file))

#define CHAR_TO_RANK(c) ((c) - 49)
#define CHAR_TO_FILE(c) ((c) - 97)

#define RANK_TO_CHAR(rank) ((rank) + 49)
#define FILE_TO_CHAR(file) ((file) +  97)

#define GET_BIT(a, bit) (((a) >> (bit)) & 1)

#define RANK_7_MASK 0xFF000000000000ULL
#define RANK_5_MASK 0xFF00000000ULL
#define RANK_4_MASK 0xFF000000ULL
#define RANK_2_MASK 0xFF00ULL

void bd_clear(Board *board);

static inline void bd_addPiece(Board *board, Piece piece, int square) {
    Bitboard position = (1ULL << square);

    board->bitboard |= position;
    PIECE_BITBOARD(board, piece) |= position;
    COLOR_BITBOARD(board, piece) |= position;

    board->pieces[square] = piece;
}

static inline void bd_movePiece(Board *board, int origin, int dest) {
    Piece piece = board->pieces[origin];
    Bitboard move = (1ULL << origin) | (1ULL << dest);

    board->bitboard ^= move;
    PIECE_BITBOARD(board, piece) ^= move;
    COLOR_BITBOARD(board, piece) ^= move;

    board->pieces[origin] = PIECE_NONE;
    board->pieces[dest] = piece;
}

static inline void bd_clearPiece(Board *board, int square) {
    Piece piece = board->pieces[square];
    if (piece == PIECE_NONE) return;

    Bitboard mask = 1ULL << square;

    board->bitboard ^= mask;
    PIECE_BITBOARD(board, piece) ^= mask;
    COLOR_BITBOARD(board, piece) ^= mask;

    board->pieces[square] = PIECE_NONE;
}

void bd_print(Board *board);
void bd_printInfo(Board *board);

void bd_squareToStr(int square, char *str);

#endif