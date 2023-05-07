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

#include "Fen.h"

void fen_load(Board *board, char *fen) {
    bd_clear(board);

    int rank = 7, file = 0;
    int i = 0;

    while(fen[i] != ' ') {
        if(fen[i] == '/') {
            rank --;
            file = 0;
        } else if(fen[i] > 64) {
            Piece piece = pc_parse(fen[i]);
            bd_addPiece(board, piece, rank * 8 + file);
            file ++;
        } else {
            file += fen[i] - 48;
        }
        i++;
    }
    i++;

    if(fen[i] == 'w') SET_TURN(board, WHITE);
    else SET_TURN(board, BLACK);
    i+= 2;

    while(fen[i] != ' ') {
        if(fen[i] == 'Q') {
            board->gameState |= WHITE_CASTLE_QUEENSIDE_RIGHT;
        } else if(fen[i] == 'q') {
            board->gameState |= BLACK_CASTLE_QUEENSIDE_RIGHT;
        } else if(fen[i] == 'K') {
            board->gameState |= WHITE_CASTLE_KINGSIDE_RIGHT;
        } else if(fen[i] == 'k') {
            board->gameState |= BLACK_CASTLE_KINGSIDE_RIGHT;
        }
        i++;
    }
    i++;

    if(fen[i] != '-') {
        SET_EP_FILE(board, fen[i] - 97);
        i++;
    }
    i+= 2;
    int len = 0;
    while(fen[i] != ' ') {
        i ++;
        len ++;
    }
    char halfMovesChar[len];
    memcpy(halfMovesChar, &fen[i-len], len);
    SET_HALFMOVE_COUNTER(board, atoi(halfMovesChar));
}