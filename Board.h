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

    Bitboard pieceBitboards[6][2];
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

#define PAWNS(board, turn) (board)->pieceBitboards[PAWN][turn]
#define KNIGHTS(board, turn) (board)->pieceBitboards[KNIGHT][turn]
#define BISHOPS(board, turn) (board)->pieceBitboards[BISHOP][turn]
#define ROOKS(board, turn) (board)->pieceBitboards[ROOK][turn]
#define QUEENS(board, turn) (board)->pieceBitboards[QUEEN][turn]
#define KINGS(board, turn) (board)->pieceBitboards[KING][turn]

#define PIECE_BITBOARD(board, piece) board->pieceBitboards[PIECE_TYPE(piece)][PIECE_COLOR(piece)]
#define COLOR_BITBOARD(board, piece) board->colorBitboards[PIECE_COLOR(piece)]

#define SET_TURN(board, turn) (board)->gameState = (((board)->gameState & 0xFEFF) | (turn << 8))
#define OPP_TURN(board) (1 - TURN(board))
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
void bd_addPiece(Board *board, Piece piece, int square);
void bd_movePiece(Board *board, int origin, int dest);
void bd_clearPiece(Board *board, int square);

void bd_print(Board *board);

void bd_squareToStr(int square, char *str);

#endif