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

#include "Move.h"

const Bitboard epRankMasks[2] = {RANK_5_MASK, RANK_4_MASK};

Move mv_fromLAN(Board *board, char *str) {
    int origin = SQUARE(CHAR_TO_RANK(str[1]), CHAR_TO_FILE(str[0]));
    int dest = SQUARE(CHAR_TO_RANK(str[3]), CHAR_TO_FILE(str[2]));
    int special = QUIET;

    if (str[4] == 'r') {
        special = ROOK_PROMOTION;
    } else if (str[4] == 'b') {
        special = BISHOP_PROMOTION;
    } else if (str[4] == 'q') {
        special = QUEEN_PROMOTION;
    } else if (str[4] == 'n') {
        special = KNIGHT_PROMOTION;
    }

    if (PIECE_TYPE(board->pieces[origin]) == PAWN && (origin - dest == 16 || origin - dest == -16))
        special = DOUBLE_PAWN_PUSH;
    else if (PIECE_TYPE(board->pieces[origin]) == PAWN && (origin - dest == 7 || origin - dest == -7 || origin - dest == 9 || origin - dest == -9) && board->pieces[dest] == PIECE_NONE)
        special = EP_CAPTURE;

    return CREATE_MOVE(origin, dest, special);
}

void mv_toLAN(Move move, char *str) {
    int origin = MOVE_ORIGIN(move);
    int dest = MOVE_DEST(move);
    int special = MOVE_SPECIAL(move);

    bd_squareToStr(origin, str);
    bd_squareToStr(dest, str + 2);

    if (special == KNIGHT_PROMOTION) {
        str[4] = 'n';
        str[5] = '\0';
    } else if (special == BISHOP_PROMOTION) {
        str[4] = 'b';
        str[5] = '\0';
    } else if (special == ROOK_PROMOTION) {
        str[4] = 'r';        
        str[5] = '\0';
    } else if (special == QUEEN_PROMOTION) {
        str[4] = 'q';        
        str[5] = '\0';
    } else {
        str[4] = '\0';
    }
}

void printBitboard2(Bitboard *bitboard) {
    for(int rank = 7; rank >= 0; rank --) {
        for(int file = 0; file < 8; file ++) {
            int i = rank*8 + file;
            if(((*bitboard >> i) & 1) == 1) printf("# ");
            else printf(". ");
        }
        printf("\n");
    }
    printf("\n");
}

void mv_make(Move move, Board *board) {

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
        bd_addPiece(board, CREATE_PIECE(turn, KNIGHT), dest);
    } else if(special == BISHOP_PROMOTION) {
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, CREATE_PIECE(turn, BISHOP), dest);
    } else if(special == ROOK_PROMOTION) {
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, CREATE_PIECE(turn, ROOK), dest);
    } else if(special == QUEEN_PROMOTION) {
        bd_clearPiece(board, dest);
        bd_clearPiece(board, origin);
        bd_addPiece(board, CREATE_PIECE(turn, QUEEN), dest);
    } else {
        bd_clearPiece(board, dest);
        bd_movePiece(board, origin, dest);
    }

    int revokedRights = 0;
    if(origin == 0 || dest == 0) revokedRights |= WHITE_CASTLE_QUEENSIDE_RIGHT;
    if(origin == 7 || dest == 7) revokedRights |= WHITE_CASTLE_KINGSIDE_RIGHT;
    if(origin == 56 || dest == 56) revokedRights |= BLACK_CASTLE_QUEENSIDE_RIGHT;
    if(origin == 63 || dest == 63) revokedRights |= BLACK_CASTLE_KINGSIDE_RIGHT;
    if(origin == 4) revokedRights |= (WHITE_CASTLE_KINGSIDE_RIGHT | WHITE_CASTLE_QUEENSIDE_RIGHT);
    if(origin == 60) revokedRights |= (BLACK_CASTLE_KINGSIDE_RIGHT | BLACK_CASTLE_QUEENSIDE_RIGHT);

    REMOVE_CASTLE_RIGHT(board, revokedRights);

    CHANGE_TURN(board);

}

void mv_unmake(MoveResult *result, Board *board) {
    int origin = MOVE_ORIGIN(result->move);
    int dest = MOVE_DEST(result->move);
    int special = MOVE_SPECIAL(result->move);

    board->gameState = result->prevGameState;

    int turn = TURN(board);
    
    if (special == EP_CAPTURE) {
        bd_movePiece(board, dest, origin);
        bd_addPiece(board, CREATE_PIECE(OPP_TURN(board), PAWN), SQUARE(RANK(origin), EP_FILE(board)));
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
        bd_addPiece(board, CREATE_PIECE(turn, PAWN), origin);
        if (result->captured != PIECE_NONE) bd_addPiece(board, result->captured, dest);
    } else {
        bd_movePiece(board, dest, origin);
        if (result->captured != PIECE_NONE) bd_addPiece(board, result->captured, dest);
    }
}