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
#define MOVE_IS_CAPTURE(move) (((move) >> 14) & 1)
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


static inline void mv_make(Move move, Board *board) {

    int origin = MOVE_ORIGIN(move);
    int dest = MOVE_DEST(move);
    int special = MOVE_SPECIAL(move);
    int epFile = EP_FILE(board);
    int turn = TURN(board);
    
    SET_EP_FILE(board, -1);

    if (MOVE_SPECIAL(move) == DOUBLE_PAWN_PUSH) {
        SET_EP_FILE(board, FILE(origin));

        if (turn == WHITE) {
            Bitboard moveBB = (1ULL << origin) | (1ULL << dest);

            board->bitboard ^= moveBB;
            board->colorBitboards[WHITE] ^= moveBB;
            board->pieceBitboards[WHITE_PAWN] ^= moveBB;
            
            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = WHITE_PAWN;
        } else {
            Bitboard moveBB = (1ULL << origin) | (1ULL << dest);
            
            board->bitboard ^= moveBB;
            board->colorBitboards[BLACK] ^= moveBB;
            board->pieceBitboards[BLACK_PAWN] ^= moveBB;

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = BLACK_PAWN;
        }
    } else if (special == EP_CAPTURE) {
        if (turn == WHITE) {
            Bitboard moveBB = (1ULL << origin) | (1ULL << dest);
            Bitboard epPawn = SQUARE_32 << epFile;

            board->bitboard ^= moveBB | epPawn;

            board->colorBitboards[WHITE] ^= moveBB;
            board->colorBitboards[BLACK] ^= epPawn;

            board->pieceBitboards[WHITE_PAWN] ^= moveBB;
            board->pieceBitboards[BLACK_PAWN] ^= epPawn;

            board->pieces[BB_GET_LSB(epPawn)] = PIECE_NONE;
            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = WHITE_PAWN;
        } else {
            Bitboard moveBB = (1ULL << origin) | (1ULL << dest);
            Bitboard epPawn = SQUARE_24 << epFile;

            board->bitboard ^= moveBB | epPawn;

            board->colorBitboards[BLACK] ^= moveBB;
            board->colorBitboards[WHITE] ^= epPawn;

            board->pieceBitboards[BLACK_PAWN] ^= moveBB;
            board->pieceBitboards[WHITE_PAWN] ^= epPawn;

            board->pieces[BB_GET_LSB(epPawn)] = PIECE_NONE;
            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = BLACK_PAWN;
        }
    } else if(special == KINGSIDE_CASTLE) {
        if(turn == WHITE) {
            board->bitboard ^= WHITE_KINGSIDE_CASTLE_BB;
            board->colorBitboards[WHITE] ^= WHITE_KINGSIDE_CASTLE_BB;

            board->pieceBitboards[WHITE_KING] ^= WHITE_KING_KINGSIDE_CASTLE_BB;
            board->pieceBitboards[WHITE_ROOK] ^= WHITE_ROOK_KINGSIDE_CASTLE_BB;

            board->pieces[6] = WHITE_KING; board->pieces[4] = PIECE_NONE;
            board->pieces[7] = PIECE_NONE; board->pieces[5] = WHITE_ROOK;
        } else {
            board->bitboard ^= BLACK_KINGSIDE_CASTLE_BB;
            board->colorBitboards[BLACK] ^= BLACK_KINGSIDE_CASTLE_BB;

            board->pieceBitboards[BLACK_KING] ^= BLACK_KING_KINGSIDE_CASTLE_BB;
            board->pieceBitboards[BLACK_ROOK] ^= BLACK_ROOK_KINGSIDE_CASTLE_BB;

            board->pieces[62] = BLACK_KING; board->pieces[60] = PIECE_NONE;
            board->pieces[61] = BLACK_ROOK; board->pieces[63] = PIECE_NONE;
        } 
    } else if(special == QUEENSIDE_CASTLE) {
        if(turn == WHITE) {
            board->bitboard ^= WHITE_QUEENSIDE_CASTLE_BB;
            board->colorBitboards[WHITE] ^= WHITE_QUEENSIDE_CASTLE_BB;

            board->pieceBitboards[WHITE_KING] ^= WHITE_KING_QUEENSIDE_CASTLE_BB;
            board->pieceBitboards[WHITE_ROOK] ^= WHITE_ROOK_QUEENSIDE_CASTLE_BB;

            board->pieces[2] = WHITE_KING; board->pieces[4] = PIECE_NONE;
            board->pieces[3] = WHITE_ROOK; board->pieces[0] = PIECE_NONE;
        } else {
            board->bitboard ^= BLACK_QUEENSIDE_CASTLE_BB;
            board->colorBitboards[BLACK] ^= BLACK_QUEENSIDE_CASTLE_BB;

            board->pieceBitboards[BLACK_KING] ^= BLACK_KING_QUEENSIDE_CASTLE_BB;
            board->pieceBitboards[BLACK_ROOK] ^= BLACK_ROOK_QUEENSIDE_CASTLE_BB;

            board->pieces[58] = BLACK_KING; board->pieces[60] = PIECE_NONE;
            board->pieces[59] = BLACK_ROOK; board->pieces[56] = PIECE_NONE;
        } 
    } else if(special == KNIGHT_PROMOTION) {
        if (turn == WHITE) {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[WHITE] ^= moveBB;

            board->pieceBitboards[WHITE_PAWN] ^= originBB;
            board->pieceBitboards[WHITE_KNIGHT] ^= destBB;

            Piece capture = board->pieces[dest];
            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[BLACK] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = WHITE_KNIGHT;
        } else {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[BLACK] ^= moveBB;

            board->pieceBitboards[BLACK_PAWN] ^= originBB;
            board->pieceBitboards[BLACK_KNIGHT] ^= destBB;

            Piece capture = board->pieces[dest];
            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[WHITE] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = BLACK_KNIGHT;
        }
    }  else if(special == BISHOP_PROMOTION) {
        if (turn == WHITE) {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[WHITE] ^= moveBB;

            board->pieceBitboards[WHITE_PAWN] ^= originBB;
            board->pieceBitboards[WHITE_BISHOP] ^= destBB;

            Piece capture = board->pieces[dest];
            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[BLACK] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = WHITE_BISHOP;
        } else {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[BLACK] ^= moveBB;

            board->pieceBitboards[BLACK_PAWN] ^= originBB;
            board->pieceBitboards[BLACK_BISHOP] ^= destBB;

            Piece capture = board->pieces[dest];
            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[WHITE] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = BLACK_BISHOP;
        }
    }  else if(special == ROOK_PROMOTION) {
        if (turn == WHITE) {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[WHITE] ^= moveBB;

            board->pieceBitboards[WHITE_PAWN] ^= originBB;
            board->pieceBitboards[WHITE_ROOK] ^= destBB;

            Piece capture = board->pieces[dest];
            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[BLACK] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = WHITE_ROOK;
        } else {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[BLACK] ^= moveBB;

            board->pieceBitboards[BLACK_PAWN] ^= originBB;
            board->pieceBitboards[BLACK_ROOK] ^= destBB;

            Piece capture = board->pieces[dest];
            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[WHITE] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = BLACK_ROOK;
        }
    }  else if(special == QUEEN_PROMOTION) {
        if (turn == WHITE) {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[WHITE] ^= moveBB;

            board->pieceBitboards[WHITE_PAWN] ^= originBB;
            board->pieceBitboards[WHITE_QUEEN] ^= destBB;

            Piece capture = board->pieces[dest];
            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[BLACK] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = WHITE_QUEEN;
        } else {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[BLACK] ^= moveBB;

            board->pieceBitboards[BLACK_PAWN] ^= originBB;
            board->pieceBitboards[BLACK_QUEEN] ^= destBB;

            Piece capture = board->pieces[dest];
            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[WHITE] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = BLACK_QUEEN;
        }
    } else {
        if (turn == WHITE) {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[WHITE] ^= moveBB;

            Piece piece = board->pieces[origin];
            Piece capture = board->pieces[dest];

            board->pieceBitboards[piece] ^= moveBB;

            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[BLACK] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = piece;
        } else {
            Bitboard originBB = (1ULL << origin);
            Bitboard destBB = (1ULL << dest);
            Bitboard moveBB = originBB | destBB;

            board->colorBitboards[BLACK] ^= moveBB;

            Piece piece = board->pieces[origin];
            Piece capture = board->pieces[dest];

            board->pieceBitboards[piece] ^= moveBB;

            if (capture != PIECE_NONE) {
                board->bitboard ^= originBB;
                board->colorBitboards[WHITE] ^= destBB;
                board->pieceBitboards[capture] ^= destBB; 
            } else {
                board->bitboard ^= moveBB;
            }

            board->pieces[origin] = PIECE_NONE;
            board->pieces[dest] = piece;
        }
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