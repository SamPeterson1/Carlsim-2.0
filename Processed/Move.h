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

#ifndef MOVE_H

#define MOVE_H
#include <stdint.h>
#include "Board.h"

typedef uint16_t Move;

#define LAN_MAX_SIZE 6

#define QUIET 0
#define DOUBLE_PAWN_PUSH 0x1000

#define KINGSIDE_CASTLE 0x2000
#define QUEENSIDE_CASTLE 0x3000

#define CAPTURE 0x4000
#define EP_CAPTURE 0x5000

#define PROMOTION 0x8000
#define KNIGHT_PROMOTION 0x8000
#define BISHOP_PROMOTION 0x9000
#define ROOK_PROMOTION 0xA000
#define QUEEN_PROMOTION 0xB000

#define CREATE_MOVE(origin, dest, special) ((origin) | ((dest) << 6) | (special))

#define MOVE_ORIGIN(move) ((move) & 0x3F)
#define MOVE_DEST(move) (((move) & 0xFC0) >> 6)
#define MOVE_IS_PROMOTION(move) (move >> 15)
#define MOVE_IS_CAPTURE(move) (((move) >> 14) & 1) //the capture flag is currently not being used
#define MOVE_SPECIAL(move) ((move) & 0xF000)

#define MOVE_RESULT(m, board) {.prevGameState = board->gameState, .captured = board->pieces[MOVE_DEST(m)], .move = (m)}

typedef struct MoveResult_s {
    uint16_t prevGameState;
    Piece captured;
    Move move;
} MoveResult;

uint16_t castleRightMasks[64];

const Piece promotionLookupsWhite[16];
const Piece promotionLookupsBlack[16];

Move mv_fromLAN(Board *board, char *str);
void mv_toLAN(Move move, char *str);
void mv_init(void);

#define WHITE_KING_KINGSIDE_CASTLE_BB ((1ULL << 6) | (1ULL << 4))
#define WHITE_ROOK_KINGSIDE_CASTLE_BB ((1ULL << 5) | (1ULL << 7))
#define WHITE_KINGSIDE_CASTLE_BB (WHITE_KING_KINGSIDE_CASTLE_BB | WHITE_ROOK_KINGSIDE_CASTLE_BB)

#define BLACK_KING_KINGSIDE_CASTLE_BB ((1ULL << 62) | (1ULL << 60))
#define BLACK_ROOK_KINGSIDE_CASTLE_BB ((1ULL << 61) | (1ULL << 63))
#define BLACK_KINGSIDE_CASTLE_BB (BLACK_KING_KINGSIDE_CASTLE_BB | BLACK_ROOK_KINGSIDE_CASTLE_BB)

#define WHITE_KING_QUEENSIDE_CASTLE_BB ((1ULL << 2) | (1ULL << 4))
#define WHITE_ROOK_QUEENSIDE_CASTLE_BB ((1ULL << 3) | (1ULL << 0))
#define WHITE_QUEENSIDE_CASTLE_BB (WHITE_KING_QUEENSIDE_CASTLE_BB | WHITE_ROOK_QUEENSIDE_CASTLE_BB)

#define BLACK_KING_QUEENSIDE_CASTLE_BB ((1ULL << 58) | (1ULL << 60))
#define BLACK_ROOK_QUEENSIDE_CASTLE_BB ((1ULL << 59) | (1ULL << 56))
#define BLACK_QUEENSIDE_CASTLE_BB (BLACK_KING_QUEENSIDE_CASTLE_BB | BLACK_ROOK_QUEENSIDE_CASTLE_BB)

static inline void mv_make_t0FAA742F(Move move, Board *board) {
    int origin = MOVE_ORIGIN(move);
    int dest = MOVE_DEST(move);
    int special = MOVE_SPECIAL(move);
    int epFile = EP_FILE(board);
    
        Piece promotion = promotionLookupsWhite[special >> 12];
        #define OUR_TURN WHITE
        #define OPP_TURN BLACK
        #define TURN_PAWN WHITE_PAWN
        #define OPP_TURN_PAWN BLACK_PAWN
        #define TURN_KING WHITE_KING
        #define TURN_ROOK WHITE_ROOK
        #define TURN_KING_KINGSIDE_CASTLE_BB WHITE_KING_KINGSIDE_CASTLE_BB
        #define TURN_ROOK_KINGSIDE_CASTLE_BB WHITE_ROOK_KINGSIDE_CASTLE_BB
        #define TURN_KING_QUEENSIDE_CASTLE_BB WHITE_KING_QUEENSIDE_CASTLE_BB
        #define TURN_ROOK_QUEENSIDE_CASTLE_BB WHITE_ROOK_QUEENSIDE_CASTLE_BB
        #define TURN_KINGSIDE_CASTLE_BB WHITE_KINGSIDE_CASTLE_BB
        #define TURN_QUEENSIDE_CASTLE_BB WHITE_QUEENSIDE_CASTLE_BB

    SET_EP_FILE(board, -1);

    if (special == QUIET) { //change if using capture flag
        Bitboard originBB = (1ULL << origin);
        Bitboard destBB = (1ULL << dest);
        Bitboard moveBB = originBB | destBB;

        board->colorBitboards[OUR_TURN] ^= moveBB;

        Piece piece = board->pieces[origin];
        Piece capture = board->pieces[dest];

        board->pieceBitboards[piece] ^= moveBB;

        if (capture != PIECE_NONE) {
            board->bitboard ^= originBB;
            board->colorBitboards[OPP_TURN] ^= destBB;
            board->pieceBitboards[capture] ^= destBB; 
        } else {
            board->bitboard ^= moveBB;
        }

        board->pieces[origin] = PIECE_NONE;
        board->pieces[dest] = piece;
    } else if (special == DOUBLE_PAWN_PUSH) {
        SET_EP_FILE(board, FILE(origin));

        Bitboard moveBB = (1ULL << origin) | (1ULL << dest);
        
        board->bitboard ^= moveBB;
        board->colorBitboards[OUR_TURN] ^= moveBB;
        board->pieceBitboards[TURN_PAWN] ^= moveBB;

        board->pieces[origin] = PIECE_NONE;
        board->pieces[dest] = TURN_PAWN;
    } else if(special == KINGSIDE_CASTLE) {
        board->bitboard ^= TURN_KINGSIDE_CASTLE_BB;
        board->colorBitboards[OUR_TURN] ^= TURN_KINGSIDE_CASTLE_BB;

        board->pieceBitboards[TURN_KING] ^= TURN_KING_KINGSIDE_CASTLE_BB;
        board->pieceBitboards[TURN_ROOK] ^= TURN_ROOK_KINGSIDE_CASTLE_BB;

            board->pieces[6] = TURN_KING; board->pieces[4] = PIECE_NONE;
            board->pieces[5] = TURN_ROOK; board->pieces[7] = PIECE_NONE; 
    } else if(special == QUEENSIDE_CASTLE) {
        board->bitboard ^= TURN_QUEENSIDE_CASTLE_BB;
        board->colorBitboards[OUR_TURN] ^= TURN_QUEENSIDE_CASTLE_BB;

        board->pieceBitboards[TURN_KING] ^= TURN_KING_QUEENSIDE_CASTLE_BB;
        board->pieceBitboards[TURN_ROOK] ^= TURN_ROOK_QUEENSIDE_CASTLE_BB;

            board->pieces[2] = TURN_KING; board->pieces[4] = PIECE_NONE;
            board->pieces[3] = TURN_ROOK; board->pieces[0] = PIECE_NONE;
    } else if (special == EP_CAPTURE) {
        Bitboard moveBB = (1ULL << origin) | (1ULL << dest);
            Bitboard epPawn = SQUARE_32 << epFile;

        board->bitboard ^= moveBB | epPawn;

        board->colorBitboards[OUR_TURN] ^= moveBB;
        board->colorBitboards[OPP_TURN] ^= epPawn;

        board->pieceBitboards[TURN_PAWN] ^= moveBB;
        board->pieceBitboards[OPP_TURN_PAWN] ^= epPawn;

        board->pieces[BB_GET_LSB(epPawn)] = PIECE_NONE;
        board->pieces[origin] = PIECE_NONE;
        board->pieces[dest] = TURN_PAWN;
    } else {
        Bitboard originBB = (1ULL << origin);
        Bitboard destBB = (1ULL << dest);
        Bitboard moveBB = originBB | destBB;

        board->colorBitboards[OUR_TURN] ^= moveBB;

        board->pieceBitboards[TURN_PAWN] ^= originBB;
        board->pieceBitboards[promotion] ^= destBB;

        Piece capture = board->pieces[dest];
        if (capture != PIECE_NONE) {
            board->bitboard ^= originBB;
            board->colorBitboards[OPP_TURN] ^= destBB;
            board->pieceBitboards[capture] ^= destBB; 
        } else {
            board->bitboard ^= moveBB;
        }

        board->pieces[origin] = PIECE_NONE;
        board->pieces[dest] = promotion;
    }

    board->gameState &= castleRightMasks[origin] & castleRightMasks[dest];

    CHANGE_TURN(board);

    #undef OUR_TURN
    #undef OPP_TURN
    #undef TURN_PAWN
    #undef OPP_TURN_PAWN
    #undef TURN_KING
    #undef TURN_ROOK
    #undef TURN_KING_KINGSIDE_CASTLE_BB
    #undef TURN_ROOK_KINGSIDE_CASTLE_BB
    #undef TURN_KING_QUEENSIDE_CASTLE_BB
    #undef TURN_ROOK_QUEENSIDE_CASTLE_BB
    #undef TURN_KINGSIDE_CASTLE_BB
    #undef TURN_QUEENSIDE_CASTLE_BB
}
static inline void mv_make_t3D5D53C5(Move move, Board *board) {
    int origin = MOVE_ORIGIN(move);
    int dest = MOVE_DEST(move);
    int special = MOVE_SPECIAL(move);
    int epFile = EP_FILE(board);
    
        Piece promotion = promotionLookupsBlack[special >> 12];
        #define OUR_TURN BLACK
        #define OPP_TURN WHITE
        #define TURN_PAWN BLACK_PAWN
        #define OPP_TURN_PAWN WHITE_PAWN
        #define TURN_KING BLACK_KING
        #define TURN_ROOK BLACK_ROOK
        #define TURN_KING_KINGSIDE_CASTLE_BB BLACK_KING_KINGSIDE_CASTLE_BB
        #define TURN_ROOK_KINGSIDE_CASTLE_BB BLACK_ROOK_KINGSIDE_CASTLE_BB
        #define TURN_KING_QUEENSIDE_CASTLE_BB BLACK_KING_QUEENSIDE_CASTLE_BB
        #define TURN_ROOK_QUEENSIDE_CASTLE_BB BLACK_ROOK_QUEENSIDE_CASTLE_BB
        #define TURN_KINGSIDE_CASTLE_BB BLACK_KINGSIDE_CASTLE_BB
        #define TURN_QUEENSIDE_CASTLE_BB BLACK_QUEENSIDE_CASTLE_BB

    SET_EP_FILE(board, -1);

    if (special == QUIET) { //change if using capture flag
        Bitboard originBB = (1ULL << origin);
        Bitboard destBB = (1ULL << dest);
        Bitboard moveBB = originBB | destBB;

        board->colorBitboards[OUR_TURN] ^= moveBB;

        Piece piece = board->pieces[origin];
        Piece capture = board->pieces[dest];

        board->pieceBitboards[piece] ^= moveBB;

        if (capture != PIECE_NONE) {
            board->bitboard ^= originBB;
            board->colorBitboards[OPP_TURN] ^= destBB;
            board->pieceBitboards[capture] ^= destBB; 
        } else {
            board->bitboard ^= moveBB;
        }

        board->pieces[origin] = PIECE_NONE;
        board->pieces[dest] = piece;
    } else if (special == DOUBLE_PAWN_PUSH) {
        SET_EP_FILE(board, FILE(origin));

        Bitboard moveBB = (1ULL << origin) | (1ULL << dest);
        
        board->bitboard ^= moveBB;
        board->colorBitboards[OUR_TURN] ^= moveBB;
        board->pieceBitboards[TURN_PAWN] ^= moveBB;

        board->pieces[origin] = PIECE_NONE;
        board->pieces[dest] = TURN_PAWN;
    } else if(special == KINGSIDE_CASTLE) {
        board->bitboard ^= TURN_KINGSIDE_CASTLE_BB;
        board->colorBitboards[OUR_TURN] ^= TURN_KINGSIDE_CASTLE_BB;

        board->pieceBitboards[TURN_KING] ^= TURN_KING_KINGSIDE_CASTLE_BB;
        board->pieceBitboards[TURN_ROOK] ^= TURN_ROOK_KINGSIDE_CASTLE_BB;

            board->pieces[62] = TURN_KING; board->pieces[60] = PIECE_NONE;
            board->pieces[61] = TURN_ROOK; board->pieces[63] = PIECE_NONE;
    } else if(special == QUEENSIDE_CASTLE) {
        board->bitboard ^= TURN_QUEENSIDE_CASTLE_BB;
        board->colorBitboards[OUR_TURN] ^= TURN_QUEENSIDE_CASTLE_BB;

        board->pieceBitboards[TURN_KING] ^= TURN_KING_QUEENSIDE_CASTLE_BB;
        board->pieceBitboards[TURN_ROOK] ^= TURN_ROOK_QUEENSIDE_CASTLE_BB;

            board->pieces[58] = TURN_KING; board->pieces[60] = PIECE_NONE;
            board->pieces[59] = TURN_ROOK; board->pieces[56] = PIECE_NONE;
    } else if (special == EP_CAPTURE) {
        Bitboard moveBB = (1ULL << origin) | (1ULL << dest);
            Bitboard epPawn = SQUARE_24 << epFile;

        board->bitboard ^= moveBB | epPawn;

        board->colorBitboards[OUR_TURN] ^= moveBB;
        board->colorBitboards[OPP_TURN] ^= epPawn;

        board->pieceBitboards[TURN_PAWN] ^= moveBB;
        board->pieceBitboards[OPP_TURN_PAWN] ^= epPawn;

        board->pieces[BB_GET_LSB(epPawn)] = PIECE_NONE;
        board->pieces[origin] = PIECE_NONE;
        board->pieces[dest] = TURN_PAWN;
    } else {
        Bitboard originBB = (1ULL << origin);
        Bitboard destBB = (1ULL << dest);
        Bitboard moveBB = originBB | destBB;

        board->colorBitboards[OUR_TURN] ^= moveBB;

        board->pieceBitboards[TURN_PAWN] ^= originBB;
        board->pieceBitboards[promotion] ^= destBB;

        Piece capture = board->pieces[dest];
        if (capture != PIECE_NONE) {
            board->bitboard ^= originBB;
            board->colorBitboards[OPP_TURN] ^= destBB;
            board->pieceBitboards[capture] ^= destBB; 
        } else {
            board->bitboard ^= moveBB;
        }

        board->pieces[origin] = PIECE_NONE;
        board->pieces[dest] = promotion;
    }

    board->gameState &= castleRightMasks[origin] & castleRightMasks[dest];

    CHANGE_TURN(board);

    #undef OUR_TURN
    #undef OPP_TURN
    #undef TURN_PAWN
    #undef OPP_TURN_PAWN
    #undef TURN_KING
    #undef TURN_ROOK
    #undef TURN_KING_KINGSIDE_CASTLE_BB
    #undef TURN_ROOK_KINGSIDE_CASTLE_BB
    #undef TURN_KING_QUEENSIDE_CASTLE_BB
    #undef TURN_ROOK_QUEENSIDE_CASTLE_BB
    #undef TURN_KINGSIDE_CASTLE_BB
    #undef TURN_QUEENSIDE_CASTLE_BB
}

static inline void mv_unmake_t6C5E82C9(MoveResult *result, Board *board) {
    int origin = MOVE_ORIGIN(result->move);
    int dest = MOVE_DEST(result->move);
    int special = MOVE_SPECIAL(result->move);
    
        Piece promotion = promotionLookupsWhite[special >> 12];
        #define OUR_TURN WHITE
        #define OPP_TURN BLACK
        #define TURN_PAWN WHITE_PAWN
        #define OPP_TURN_PAWN BLACK_PAWN
        #define TURN_KING WHITE_KING
        #define TURN_ROOK WHITE_ROOK
        #define TURN_KING_KINGSIDE_CASTLE_BB WHITE_KING_KINGSIDE_CASTLE_BB
        #define TURN_ROOK_KINGSIDE_CASTLE_BB WHITE_ROOK_KINGSIDE_CASTLE_BB
        #define TURN_KING_QUEENSIDE_CASTLE_BB WHITE_KING_QUEENSIDE_CASTLE_BB
        #define TURN_ROOK_QUEENSIDE_CASTLE_BB WHITE_ROOK_QUEENSIDE_CASTLE_BB
        #define TURN_KINGSIDE_CASTLE_BB WHITE_KINGSIDE_CASTLE_BB
        #define TURN_QUEENSIDE_CASTLE_BB WHITE_QUEENSIDE_CASTLE_BB

    board->gameState = result->prevGameState;
    
    if (special == QUIET || special == DOUBLE_PAWN_PUSH) {
        Bitboard originBB = (1ULL << origin);
        Bitboard destBB = (1ULL << dest);
        Bitboard moveBB = originBB | destBB;

        board->colorBitboards[OUR_TURN] ^= moveBB;

        Piece piece = board->pieces[dest];
        Piece capture = result->captured;

        board->pieceBitboards[piece] ^= moveBB;

        if (capture != PIECE_NONE) {
            board->bitboard ^= originBB;
            board->colorBitboards[OPP_TURN] ^= destBB;
            board->pieceBitboards[capture] ^= destBB; 
        } else {
            board->bitboard ^= moveBB;
        }

        board->pieces[origin] = piece;
        board->pieces[dest] = capture;
    } else if(special == KINGSIDE_CASTLE) {
        board->bitboard ^= TURN_KINGSIDE_CASTLE_BB;
        board->colorBitboards[OUR_TURN] ^= TURN_KINGSIDE_CASTLE_BB;

        board->pieceBitboards[TURN_KING] ^= TURN_KING_KINGSIDE_CASTLE_BB;
        board->pieceBitboards[TURN_ROOK] ^= TURN_ROOK_KINGSIDE_CASTLE_BB;

            board->pieces[6] = PIECE_NONE; board->pieces[4] = TURN_KING;
            board->pieces[5] = PIECE_NONE; board->pieces[7] = TURN_ROOK; 
    } else if(special == QUEENSIDE_CASTLE) {
        board->bitboard ^= TURN_QUEENSIDE_CASTLE_BB;
        board->colorBitboards[OUR_TURN] ^= TURN_QUEENSIDE_CASTLE_BB;

        board->pieceBitboards[TURN_KING] ^= TURN_KING_QUEENSIDE_CASTLE_BB;
        board->pieceBitboards[TURN_ROOK] ^= TURN_ROOK_QUEENSIDE_CASTLE_BB;

            board->pieces[2] = PIECE_NONE; board->pieces[4] = TURN_KING;
            board->pieces[3] = PIECE_NONE; board->pieces[0] = TURN_ROOK;
    }  else if (special == EP_CAPTURE) {
        Bitboard moveBB = (1ULL << origin) | (1ULL << dest);
            Bitboard epPawn = SQUARE_32 << EP_FILE(board);

        board->bitboard ^= moveBB | epPawn;

        board->colorBitboards[OUR_TURN] ^= moveBB;
        board->colorBitboards[OPP_TURN] ^= epPawn;

        board->pieceBitboards[TURN_PAWN] ^= moveBB;
        board->pieceBitboards[OPP_TURN_PAWN] ^= epPawn;

        board->pieces[BB_GET_LSB(epPawn)] = OPP_TURN_PAWN;
        board->pieces[origin] = TURN_PAWN;
        board->pieces[dest] = PIECE_NONE;
    } else {
        Bitboard originBB = (1ULL << origin);
        Bitboard destBB = (1ULL << dest);
        Bitboard moveBB = originBB | destBB;

        board->colorBitboards[OUR_TURN] ^= moveBB;

        board->pieceBitboards[TURN_PAWN] ^= originBB;
        board->pieceBitboards[promotion] ^= destBB;

        if (result->captured != PIECE_NONE) {
            board->bitboard ^= originBB;
            board->colorBitboards[OPP_TURN] ^= destBB;
            board->pieceBitboards[result->captured] ^= destBB; 
        } else {
            board->bitboard ^= moveBB;
        }

        board->pieces[origin] = TURN_PAWN;
        board->pieces[dest] = result->captured;
    }

    #undef OUR_TURN
    #undef OPP_TURN
    #undef TURN_PAWN
    #undef OPP_TURN_PAWN
    #undef TURN_KING
    #undef TURN_ROOK
    #undef TURN_KING_KINGSIDE_CASTLE_BB
    #undef TURN_ROOK_KINGSIDE_CASTLE_BB
    #undef TURN_KING_QUEENSIDE_CASTLE_BB
    #undef TURN_ROOK_QUEENSIDE_CASTLE_BB
    #undef TURN_KINGSIDE_CASTLE_BB
    #undef TURN_QUEENSIDE_CASTLE_BB
}
static inline void mv_unmake_tF229BC95(MoveResult *result, Board *board) {
    int origin = MOVE_ORIGIN(result->move);
    int dest = MOVE_DEST(result->move);
    int special = MOVE_SPECIAL(result->move);
    
        Piece promotion = promotionLookupsBlack[special >> 12];
        #define OUR_TURN BLACK
        #define OPP_TURN WHITE
        #define TURN_PAWN BLACK_PAWN
        #define OPP_TURN_PAWN WHITE_PAWN
        #define TURN_KING BLACK_KING
        #define TURN_ROOK BLACK_ROOK
        #define TURN_KING_KINGSIDE_CASTLE_BB BLACK_KING_KINGSIDE_CASTLE_BB
        #define TURN_ROOK_KINGSIDE_CASTLE_BB BLACK_ROOK_KINGSIDE_CASTLE_BB
        #define TURN_KING_QUEENSIDE_CASTLE_BB BLACK_KING_QUEENSIDE_CASTLE_BB
        #define TURN_ROOK_QUEENSIDE_CASTLE_BB BLACK_ROOK_QUEENSIDE_CASTLE_BB
        #define TURN_KINGSIDE_CASTLE_BB BLACK_KINGSIDE_CASTLE_BB
        #define TURN_QUEENSIDE_CASTLE_BB BLACK_QUEENSIDE_CASTLE_BB

    board->gameState = result->prevGameState;
    
    if (special == QUIET || special == DOUBLE_PAWN_PUSH) {
        Bitboard originBB = (1ULL << origin);
        Bitboard destBB = (1ULL << dest);
        Bitboard moveBB = originBB | destBB;

        board->colorBitboards[OUR_TURN] ^= moveBB;

        Piece piece = board->pieces[dest];
        Piece capture = result->captured;

        board->pieceBitboards[piece] ^= moveBB;

        if (capture != PIECE_NONE) {
            board->bitboard ^= originBB;
            board->colorBitboards[OPP_TURN] ^= destBB;
            board->pieceBitboards[capture] ^= destBB; 
        } else {
            board->bitboard ^= moveBB;
        }

        board->pieces[origin] = piece;
        board->pieces[dest] = capture;
    } else if(special == KINGSIDE_CASTLE) {
        board->bitboard ^= TURN_KINGSIDE_CASTLE_BB;
        board->colorBitboards[OUR_TURN] ^= TURN_KINGSIDE_CASTLE_BB;

        board->pieceBitboards[TURN_KING] ^= TURN_KING_KINGSIDE_CASTLE_BB;
        board->pieceBitboards[TURN_ROOK] ^= TURN_ROOK_KINGSIDE_CASTLE_BB;

            board->pieces[62] = PIECE_NONE; board->pieces[60] = TURN_KING;
            board->pieces[61] = PIECE_NONE; board->pieces[63] = TURN_ROOK;
    } else if(special == QUEENSIDE_CASTLE) {
        board->bitboard ^= TURN_QUEENSIDE_CASTLE_BB;
        board->colorBitboards[OUR_TURN] ^= TURN_QUEENSIDE_CASTLE_BB;

        board->pieceBitboards[TURN_KING] ^= TURN_KING_QUEENSIDE_CASTLE_BB;
        board->pieceBitboards[TURN_ROOK] ^= TURN_ROOK_QUEENSIDE_CASTLE_BB;

            board->pieces[58] = PIECE_NONE; board->pieces[60] = TURN_KING;
            board->pieces[59] = PIECE_NONE; board->pieces[56] = TURN_ROOK;
    }  else if (special == EP_CAPTURE) {
        Bitboard moveBB = (1ULL << origin) | (1ULL << dest);
            Bitboard epPawn = SQUARE_24 << EP_FILE(board);

        board->bitboard ^= moveBB | epPawn;

        board->colorBitboards[OUR_TURN] ^= moveBB;
        board->colorBitboards[OPP_TURN] ^= epPawn;

        board->pieceBitboards[TURN_PAWN] ^= moveBB;
        board->pieceBitboards[OPP_TURN_PAWN] ^= epPawn;

        board->pieces[BB_GET_LSB(epPawn)] = OPP_TURN_PAWN;
        board->pieces[origin] = TURN_PAWN;
        board->pieces[dest] = PIECE_NONE;
    } else {
        Bitboard originBB = (1ULL << origin);
        Bitboard destBB = (1ULL << dest);
        Bitboard moveBB = originBB | destBB;

        board->colorBitboards[OUR_TURN] ^= moveBB;

        board->pieceBitboards[TURN_PAWN] ^= originBB;
        board->pieceBitboards[promotion] ^= destBB;

        if (result->captured != PIECE_NONE) {
            board->bitboard ^= originBB;
            board->colorBitboards[OPP_TURN] ^= destBB;
            board->pieceBitboards[result->captured] ^= destBB; 
        } else {
            board->bitboard ^= moveBB;
        }

        board->pieces[origin] = TURN_PAWN;
        board->pieces[dest] = result->captured;
    }

    #undef OUR_TURN
    #undef OPP_TURN
    #undef TURN_PAWN
    #undef OPP_TURN_PAWN
    #undef TURN_KING
    #undef TURN_ROOK
    #undef TURN_KING_KINGSIDE_CASTLE_BB
    #undef TURN_ROOK_KINGSIDE_CASTLE_BB
    #undef TURN_KING_QUEENSIDE_CASTLE_BB
    #undef TURN_ROOK_QUEENSIDE_CASTLE_BB
    #undef TURN_KINGSIDE_CASTLE_BB
    #undef TURN_QUEENSIDE_CASTLE_BB
}

#endif
