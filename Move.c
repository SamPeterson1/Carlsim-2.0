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

void mv_init(void) {
    for (int square = 0; square < 64; square ++) {
        uint16_t revokedRights = 0;

        if(square == 0) revokedRights |= WHITE_CASTLE_QUEENSIDE_RIGHT;
        if(square == 7) revokedRights |= WHITE_CASTLE_KINGSIDE_RIGHT;
        if(square == 56) revokedRights |= BLACK_CASTLE_QUEENSIDE_RIGHT;
        if(square == 63) revokedRights |= BLACK_CASTLE_KINGSIDE_RIGHT;
        if(square == 4) revokedRights |= (WHITE_CASTLE_KINGSIDE_RIGHT | WHITE_CASTLE_QUEENSIDE_RIGHT);
        if(square == 60) revokedRights |= (BLACK_CASTLE_KINGSIDE_RIGHT | BLACK_CASTLE_QUEENSIDE_RIGHT);

        castleRightMasks[square] = ~revokedRights;
    }
}

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
