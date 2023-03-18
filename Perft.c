#include "Perft.h"
#include <sys/time.h>

unsigned long totalTime = 0;
long totalMoves = 0;

unsigned long micros(void) {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return 1000000 * tv.tv_sec + tv.tv_usec;
}

long r_perft(Board *board, int depth, int originalDepth) {

    Move moves[MG_MAX_MOVES];
    unsigned long foo = micros();
    int movec = mg_gen(board, moves);
    totalTime += micros() - foo;
    totalMoves += movec;

    if (depth == 1) 
        return movec;

    long numPositions = 0;

    for (int i = 0; i < movec; i ++) {
        MoveResult result = MOVE_RESULT(moves[i], board);

        mv_make(moves[i], board);
        long n = r_perft(board, depth - 1, originalDepth);

        if (depth == originalDepth) {
            char str[LAN_MAX_SIZE];
            mv_toLAN(moves[i], str);
            printf("%s: %ld\n", str, n);
        }

        numPositions += n;
        mv_unmake(&result, board);
    }

    return numPositions;
}

int perft(Board *board, int depth) {
    long numPositions = r_perft(board, depth, depth);
    printf("Positions searched: %ld\n", numPositions);
    printf("Average move generation time: %f moves/sec\n", (double)totalMoves/totalTime*1.0E6);
    return numPositions;
}