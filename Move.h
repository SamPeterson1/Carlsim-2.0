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
#define MOVE_IS_CAPTURE(move) (((move) >> 14) & 1)
#define MOVE_IS_PROMOTION(move) (((move) >> 15) & 1)
#define MOVE_SPECIAL(move) ((move) & 0xF000)

#define MOVE_RESULT(m, board) {.prevGameState = board->gameState, .captured = board->pieces[MOVE_DEST(m)], .move = (m)}

typedef struct MoveResult_s {
    uint16_t prevGameState;
    Piece captured;
    Move move;
} MoveResult;

uint16_t castleRightMasks[64];

Move mv_fromLAN(Board *board, char *str);
void mv_toLAN(Move move, char *str);
void mv_init(void);

/*
static inline void mv_make(Move move, Board *board) {
    int origin = MOVE_ORIGIN(move);
    int dest = MOVE_DEST(move);
    int special = MOVE_SPECIAL(move);

    SET_EP_FILE(board, -1);

    if (special == QUIET) {
        bd_clearPiece(board, dest);
        bd_movePiece(board, origin, dest);
    } else if (special == DOUBLE_PAWN_PUSH) {
        SET_EP_FILE(board, FILE(origin));
        bd_movePiece(board, origin, dest);
    } else if (special == EP_CAPTURE) {
        bd_clearPiece(board, SQUARE(RANK(origin), EP_FILE(board)));
        bd_movePiece(board, origin, dest);
    } else if(special == KINGSIDE_CASTLE) {
        if(TURN(board) == WHITE) {
            bd_movePiece(board, 7, 5);
            REMOVE_CASTLE_RIGHT(board, WHITE_CASTLE_KINGSIDE_RIGHT | WHITE_CASTLE_QUEENSIDE_RIGHT);
        } else {
            bd_movePiece(board, 63, 61);
            REMOVE_CASTLE_RIGHT(board, BLACK_CASTLE_KINGSIDE_RIGHT | BLACK_CASTLE_QUEENSIDE_RIGHT);
        } 

        bd_movePiece(board, origin, dest);
    } else if(special == QUEENSIDE_CASTLE) {
        if(TURN(board) == WHITE) {
            bd_movePiece(board, 0, 3);
            REMOVE_CASTLE_RIGHT(board, WHITE_CASTLE_KINGSIDE_RIGHT | WHITE_CASTLE_QUEENSIDE_RIGHT);
        } else {
            bd_movePiece(board, 56, 59);
            REMOVE_CASTLE_RIGHT(board, BLACK_CASTLE_KINGSIDE_RIGHT | BLACK_CASTLE_QUEENSIDE_RIGHT);
        }

        bd_movePiece(board, origin, dest);
    } else if(special == KNIGHT_PROMOTION) {
        //maybe add function to clear multiple pieces
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, TURN(board) | KNIGHT, dest);
    } else if(special == BISHOP_PROMOTION) {
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, TURN(board) | BISHOP, dest);
    } else if(special == ROOK_PROMOTION) {
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, TURN(board) | ROOK, dest);
    } else if(special == QUEEN_PROMOTION) {
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, TURN(board) | QUEEN, dest);
    }

    //board->gameState &= castleRightMasks[origin] & castleRightMasks[dest];

    CHANGE_TURN(board);
}

static inline void mv_unmake(MoveResult *result, Board *board) {
    int origin = MOVE_ORIGIN(result->move);
    int dest = MOVE_DEST(result->move);
    int special = MOVE_SPECIAL(result->move);

    board->gameState = result->prevGameState;

    int turn = TURN(board);
    
    if (special == EP_CAPTURE) {
        bd_movePiece(board, dest, origin);
        bd_addPiece(board, PAWN | (1 - turn), SQUARE(RANK(origin), EP_FILE(board)));
    } else if(special == KINGSIDE_CASTLE) {
        bd_movePiece(board, dest, origin);
        if(turn == WHITE) {
            bd_movePiece(board, 5, 7);
        } else {
            bd_movePiece(board, 61, 63);
        }  
    } else if(special == QUEENSIDE_CASTLE) {
        bd_movePiece(board, dest, origin);
        if(turn == WHITE) {
            bd_movePiece(board, 3, 0);
        } else {
            bd_movePiece(board, 59, 56);
        }
    } else if(MOVE_IS_PROMOTION(result->move)) {
        bd_clearPiece(board, dest);
        bd_addPiece(board, PAWN | turn, origin);
        if (result->captured != PIECE_NONE) bd_addPiece(board, result->captured, dest);
    } else {
        bd_movePiece(board, dest, origin);
        if (result->captured != PIECE_NONE) bd_addPiece(board, result->captured, dest);
    }
}
*/

static inline void mv_make(Move move, Board *board) {

    int origin = MOVE_ORIGIN(move);
    int dest = MOVE_DEST(move);
    int special = MOVE_SPECIAL(move);
    int epFile = EP_FILE(board);
    int turn = TURN(board);
    SET_EP_FILE(board, -1);

    if (MOVE_SPECIAL(move) == DOUBLE_PAWN_PUSH) {
        SET_EP_FILE(board, FILE(origin));

        bd_movePiece(board, origin, dest);
    } else if (special == EP_CAPTURE) {
        bd_clearPiece(board, SQUARE(RANK(origin), epFile));
        bd_movePiece(board, origin, dest);
    } else if(special == KINGSIDE_CASTLE) {
        if(turn == WHITE) {
            bd_movePiece(board, 7, 5);
            REMOVE_CASTLE_RIGHT(board, WHITE_CASTLE_KINGSIDE_RIGHT | WHITE_CASTLE_QUEENSIDE_RIGHT);
        } else {
            bd_movePiece(board, 63, 61);
            REMOVE_CASTLE_RIGHT(board, BLACK_CASTLE_KINGSIDE_RIGHT | BLACK_CASTLE_QUEENSIDE_RIGHT);
        } 

        bd_movePiece(board, origin, dest);
    } else if(special == QUEENSIDE_CASTLE) {
        if(turn == WHITE) {
            bd_movePiece(board, 0, 3);
            REMOVE_CASTLE_RIGHT(board, WHITE_CASTLE_KINGSIDE_RIGHT | WHITE_CASTLE_QUEENSIDE_RIGHT);
        } else {
            bd_movePiece(board, 56, 59);
            REMOVE_CASTLE_RIGHT(board, BLACK_CASTLE_KINGSIDE_RIGHT | BLACK_CASTLE_QUEENSIDE_RIGHT);
        }

        bd_movePiece(board, origin, dest);
    } else if(special == KNIGHT_PROMOTION) {
        //maybe add function to clear multiple pieces
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, turn | KNIGHT, dest);
    } else if(special == BISHOP_PROMOTION) {
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, turn | BISHOP, dest);
    } else if(special == ROOK_PROMOTION) {
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, turn | ROOK, dest);
    } else if(special == QUEEN_PROMOTION) {
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, turn | QUEEN, dest);
    } else {
        bd_clearPiece(board, dest);
        bd_movePiece(board, origin, dest);
    }

    board->gameState &= castleRightMasks[origin] & castleRightMasks[dest];

    CHANGE_TURN(board);

}

static inline void mv_unmake(MoveResult *result, Board *board) {
    int origin = MOVE_ORIGIN(result->move);
    int dest = MOVE_DEST(result->move);
    int special = MOVE_SPECIAL(result->move);

    board->gameState = result->prevGameState;

    int turn = TURN(board);
    
    if (special == EP_CAPTURE) {
        bd_movePiece(board, dest, origin);
        bd_addPiece(board, (1 - TURN(board)) | PAWN, SQUARE(RANK(origin), EP_FILE(board)));
    } else if(special == KINGSIDE_CASTLE) {
        bd_movePiece(board, dest, origin);
        if(turn == WHITE) {
            bd_movePiece(board, 5, 7);
        } else {
            bd_movePiece(board, 61, 63);
        }  
    } else if(special == QUEENSIDE_CASTLE) {
        bd_movePiece(board, dest, origin);
        if(turn == WHITE) {
            bd_movePiece(board, 3, 0);
        } else {
            bd_movePiece(board, 59, 56);
        }
    } else if(MOVE_IS_PROMOTION(result->move)) {
        bd_clearPiece(board, dest);
        bd_addPiece(board, turn | PAWN, origin);
        if (result->captured != PIECE_NONE) bd_addPiece(board, result->captured, dest);
    } else {
        bd_movePiece(board, dest, origin);
        if (result->captured != PIECE_NONE) bd_addPiece(board, result->captured, dest);
    }
}

#endif