#include "Search.h"

int positionsSearched = 0;
uint16_t bestMove;

int contains(uint16_t *arr, uint16_t a, int len) {
    for(int i = 0; i < len; i ++) {
        if(arr[i] == a) return TRUE;
    }
    return FALSE;
}

/*template<turn: WHITE | BLACK>*/
void orderMoves(Board *board, Move *moves, int numMoves) {

    int moveEvals[numMoves];
    /*use<evalMove>*/
    for(int i = 0; i < numMoves; i ++) {
        /*set<turn: turn$>*/
        moveEvals[i] = evalMove(board, moves[i]);
    }   

    //insertion sort
    for(int i = 1; i < numMoves; i ++) {
        int j = i;
        int tmp;
        while(j > 0 && moveEvals[j] > moveEvals[j-1]) {
            tmp = moveEvals[j];
            moveEvals[j] = moveEvals[j-1];
            moveEvals[j-1] = tmp;
            tmp = moves[j];
            moves[j] = moves[j-1];
            moves[j-1] = tmp;
            j--;
        }
    }
} 

/*template<turn: WHITE | BLACK>*/
int quiescense(Board *board, int alpha, int beta, int d) {
    positionsSearched ++;

    /*use<evalPosition>*/
    /*set<turn: turn$>*/
    int eval = evalPosition(board);

    if(eval >= beta) return beta;
    if(eval > alpha) alpha = eval;

    Move moves[256];

    /*use<mg_gen>*/
    /*set<turn: turn$, genType: CAPTURES>*/
    int numMoves = mg_gen(board, moves);

    /*use<orderMoves>*/
    /*set<turn: turn$>*/
    orderMoves(board, moves, numMoves);
    for(int i = 0; i < numMoves; i ++) {
        MoveResult result = MOVE_RESULT(moves[i], board);
        /*use<mv_make>*/
        /*set<turn: turn$>*/
        mv_make(moves[i], board);

        /*use<quiescense>*/
        /*set<turn: turn$(WHITE -> BLACK | BLACK -> WHITE)>*/
        eval = -quiescense(board, -beta, -alpha, d + 1);

        /*use<mv_unmake>*/
        /*set<turn: turn$>*/
        mv_unmake(&result, board);

        if(eval >= beta) return beta;
        if(eval >= alpha) {
            alpha = eval;
        }
    }

    
    return alpha;
}
/*endtemplate*/

/*template<turn: WHITE | BLACK>*/
int negamax(Board *board, int depth, int alpha, int beta) {
    if(depth == 0) {
        /*use<quiescense>*/
        /*set<turn: turn$>*/
        return quiescense(board, alpha, beta, 0);
    } else {
        Move moves[256];
        Move bestMoveInPos = MOVE_NONE;

        /*use<mg_gen>*/
        /*set<turn: turn$, genType: ALL>*/
        int numMoves = mg_gen(board, moves);

        /*use<orderMoves>*/
        /*set<turn: turn$>*/
        orderMoves(board, moves, numMoves);
        if(board->mateStatus == CHECKMATE) {
            return -EVAL_INF;
        } else if(board->mateStatus == STALEMATE) {
            return 0;
        }

        for(int i = 0; i < numMoves; i ++) {
            MoveResult result = MOVE_RESULT(moves[i], board);
            /*use<mv_make>*/
            /*set<turn: turn$>*/
            mv_make(moves[i], board);

            /*use<negamax>*/
            /*set<turn: turn$(WHITE -> BLACK | BLACK -> WHITE)>*/
            int eval = -negamax(board, depth-1, -beta, -alpha);

            /*use<mv_unmake>*/
            /*set<turn: turn$>*/
            mv_unmake(&result, board);

            if(eval >= beta) {
                return beta;
            }
            if(eval > alpha) {
                if(depth == 6) bestMove = moves[i];
                alpha = eval;
            }
        }

        //tt_storeEval(&g_board.zobrist, alpha, depth, evalType, bestMoveInPos);
        return alpha;
    }
}
/*endtemplate*/


/*template<turn: WHITE | BLACK>*/
Move findBestMove(Board *board, int depth) {
    
    //uint16_t bookMove = bk_getMove(&g_board.zobrist);
    positionsSearched = 0;
    //if(bookMove != 0) {
    //    printf("Found position in book");
    //    return bookMove;
    //} else {
   //     printf("Starting search.");
    //}

    //for(int d = 1; d <= depth; d ++) {
    /*use<negamax>*/
    /*set<turn: turn$>*/
    negamax(board, depth, -EVAL_INF-1, EVAL_INF+1);
    printf("Depth searched: %d\n", depth);
    //}

    printf("\nPositions searched: %d\n", positionsSearched);
    return bestMove;
}
/*endtemplate*/