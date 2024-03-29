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

#include "uci.h"

Board board;
int hasInitialized = FALSE;

void cmd_uci(void) {
    printf("id name Carlsim 2.0\n");
    printf("id author Sam Peterson\n");
    printf("uciok\n");
}

void cmd_isready(void) {
    if (!hasInitialized) {
        z_init();
        mv_init();
        mg_init();

        hasInitialized = TRUE;
    }

    printf("readyok\n");
}

void cmd_go(char **args) {
    Move bestMove;

    /*use<findBestMove>*/
    if (TURN(&board) == WHITE) {
        /*set<turn: WHITE>*/
        bestMove = findBestMove(&board, 5);
    } else {
        /*set<turn: BLACK>*/
        bestMove = findBestMove(&board, 5);
    }
    char lan[LAN_MAX_SIZE];
    mv_toLAN(bestMove, lan);

    printf("bestmove %s\n", lan);
    printf("readyok\n");
}


void cmd_position(char **args, int argc) {
    int nextArg = 0;
    
    if (strcmp(args[1], "fen") == 0) {
        char fen[256];
        sprintf(fen, "%s %s %s %s %s", args[2], args[3], args[4], args[5], args[6]);
        fen_load(&board, fen);

        nextArg = 8;
    } else if (strcmp(args[1], "startpos") == 0) {
        fen_load(&board, STARTING_FEN); 
        nextArg = 2;
    }

    if (nextArg < argc && strcmp(args[nextArg], "moves") == 0) {
        for (int i = nextArg + 1; i < argc; i ++) {
            /*use<mv_make>*/
            if (TURN(&board) == WHITE)
                /*set<turn: WHITE>*/
                mv_make(mv_fromLAN(&board, args[i]), &board);
            else
                /*set<turn: BLACK>*/
                mv_make(mv_fromLAN(&board, args[i]), &board);
        }
    }

    board.zobrist = z_getKey(&board);
}

void cmd_move(char** args) {
    Move move = mv_fromLAN(&board, args[1]);
    if (TURN(&board) == WHITE)
        /*use<mv_make>*/
        /*set<turn: WHITE>*/
        mv_make(move, &board);
    else
        /*use<mv_make>*/
        /*set<turn: BLACK>*/
        mv_make(move, &board);

    ZobristKey key = z_getKey(&board);
    if(key != board.zobrist)
        printf("Error. Keys don't match.\n");
    
    printf("Key: %ld\n", board.zobrist);
}

void cmd_perft(char **args) {
    int depth = atoi(args[1]);
    perft(&board, depth);
}

void cmd_board(void) {
    bd_print(&board);
}

int uci_exec(char *cmd) {
    char **args = initBuffer();
    int argc = 0;

    parse(cmd, args, &argc);

    if (strcmp(args[0], "quit") == 0)
        return 1;
    if (strcmp(args[0], "uci") == 0) {
        cmd_uci();
    } else if (strcmp(args[0], "isready") == 0) {
        cmd_isready();
    } else if (strcmp(args[0], "go") == 0) {
        cmd_go(args);
    } else if (strcmp(args[0], "position") == 0) {
        cmd_position(args, argc);
    } else if (strcmp(args[0], "board") == 0) {
        cmd_board();
    } else if (strcmp(args[0], "move") == 0) {
        cmd_move(args);
    } else if (strcmp(args[0], "perft") == 0) {
        cmd_perft(args);
    }

    return 0;
}

void uci_perfTest() {
    mg_init();
    mv_init();

    fen_load(&board, STARTING_FEN);
    perft(&board, 6);
}