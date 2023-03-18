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
        mg_init();
        hasInitialized = TRUE;
    }

    printf("readyok\n");
}

void cmd_go(char **args) {
    Move moves[MG_MAX_MOVES];
    int movec = mg_gen(&board, moves);
    Move move = moves[rand() % movec];
    char lan[LAN_MAX_SIZE];
    mv_toLAN(move, lan);

    printf("bestmove %s\n", lan);
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
        for (int i = nextArg + 1; i < argc; i ++)
            mv_make(mv_fromLAN(&board, args[i]), &board);
    }
}

void cmd_move(char** args) {
    Move move = mv_fromLAN(&board, args[1]);
    mv_make(move, &board);
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