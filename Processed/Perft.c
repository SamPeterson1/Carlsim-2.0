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

#include "Perft.h"
#include <sys/time.h>

long movesGenerated = 0;

unsigned long micros(void) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return 1000000 * tv.tv_sec + tv.tv_usec;
}

long r_perft_t58A37877(Board *board, int depth, int originalDepth) {
    Move moves[MG_MAX_MOVES];

    int movec = mg_gen_t4FC832A0(board, moves);
    
    movesGenerated += movec;

    if (depth == 1) 
        return movec;

    long numPositions = 0;

    for (int i = 0; i < movec; i ++) {
        MoveResult result = MOVE_RESULT(moves[i], board);

        mv_make_tA9984799(moves[i], board);

        long n = 0;

        n = r_perft_tCC45A78A(board, depth - 1, originalDepth);

        if (depth == originalDepth) {
            char str[LAN_MAX_SIZE];
            mv_toLAN(moves[i], str);
            printf("%s: %ld\n", str, n);
        }

        numPositions += n;

        mv_unmake_t9E9FC584(&result, board);
    }

    return numPositions;
}
long r_perft_tCC45A78A(Board *board, int depth, int originalDepth) {
    Move moves[MG_MAX_MOVES];

    int movec = mg_gen_t1A9921E3(board, moves);
    
    movesGenerated += movec;

    if (depth == 1) 
        return movec;

    long numPositions = 0;

    for (int i = 0; i < movec; i ++) {
        MoveResult result = MOVE_RESULT(moves[i], board);

        mv_make_tE19B4D14(moves[i], board);

        long n = 0;

        n = r_perft_t58A37877(board, depth - 1, originalDepth);

        if (depth == originalDepth) {
            char str[LAN_MAX_SIZE];
            mv_toLAN(moves[i], str);
            printf("%s: %ld\n", str, n);
        }

        numPositions += n;

        mv_unmake_t96A0C15E(&result, board);
    }

    return numPositions;
}

int perft(Board *board, int depth) {
    movesGenerated = 0;

    long startTime = micros();
    long numPositions = 0;

    if (TURN(board) == WHITE) {
        numPositions = r_perft_t58A37877(board, depth, depth);
    } else {
        numPositions = r_perft_tCC45A78A(board, depth, depth);
    }

    long timeSpent = micros() - startTime;

    printf("Positions searched: %ld\n", numPositions);
    printf("Average move generation time: %f moves/sec\n", (double)movesGenerated/timeSpent*1.0E6);
    //printf("Time spent: %f sec\n", timeSpent*1.0E-6);

    return numPositions;
}
