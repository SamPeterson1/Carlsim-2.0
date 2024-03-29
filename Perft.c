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

/*template<turn: WHITE | BLACK>*/
long r_perft(Board *board, int depth, int originalDepth) {
    Move moves[MG_MAX_MOVES];

    ASSERT(z_getKey(board) == board->zobrist, "Zobrist key doesn't match");

    /*use<mg_gen>*/
    /*set<turn: turn$, genType: ALL>*/
    int movec = mg_gen(board, moves);

    movesGenerated += movec;

    if (depth == 1) 
        return movec;

    long numPositions = 0;

    for (int i = 0; i < movec; i ++) {
        MoveResult result = MOVE_RESULT(moves[i], board);

        /*use<mv_make>*/
        /*set<turn: turn$>*/
        mv_make(moves[i], board);

        long n = 0;

        /*use<r_perft>*/
        /*set<turn: turn$(WHITE -> BLACK | BLACK -> WHITE)>*/
        n = r_perft(board, depth - 1, originalDepth);

        if (depth == originalDepth) {
            char str[LAN_MAX_SIZE];
            mv_toLAN(moves[i], str);
            printf("%s: %ld\n", str, n);
        }

        numPositions += n;

        /*use<mv_unmake>*/
        /*set<turn: turn$>*/
        mv_unmake(&result, board);
    }

    return numPositions;
}
/*endtemplate*/

int perft(Board *board, int depth) {
    movesGenerated = 0;

    long startTime = micros();
    long numPositions = 0;

    /*use<r_perft>*/
    if (TURN(board) == WHITE) {
        /*set<turn: WHITE>*/
        numPositions = r_perft(board, depth, depth);
    } else {
        /*set<turn: BLACK>*/
        numPositions = r_perft(board, depth, depth);
    }

    long timeSpent = micros() - startTime;

    printf("Positions searched: %ld\n", numPositions);
    printf("Average move generation time: %f moves/sec\n", (double)movesGenerated/timeSpent*1.0E6);
    //printf("Time spent: %f sec\n", timeSpent*1.0E-6);

    return numPositions;
}