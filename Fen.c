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
        printf("Ep file: %d\n", fen[i] - 97);
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