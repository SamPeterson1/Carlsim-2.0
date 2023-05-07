#include "Search.h"

Move bestMove;
Move rootMoves[MG_MAX_MOVES];
int rootMoveEvals[MG_MAX_MOVES];
int numRootMoves;
int searchNodes;
int quiescenceNodes;

int contains(uint16_t *arr, uint16_t a, int len) {
    for(int i = 0; i < len; i ++) {
        if(arr[i] == a) return TRUE;
    }
    return FALSE;
}

void insertionSort(Move *moves, int *moveEvals, int numMoves) {
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
void orderMoves(Board *board, Move *moves, int numMoves, Move ttMove) {
    int moveEvals[numMoves];
    /*use<evalMove>*/
    for(int i = 0; i < numMoves; i ++) {
        if (moves[i] == ttMove) {
            moveEvals[i] = EVAL_INF;
        } else {
            /*set<turn: turn$>*/
            moveEvals[i] = evalMove(board, moves[i]);
        }
    }   

    insertionSort(moves, moveEvals, numMoves);
} 

/*template<turn: WHITE | BLACK>*/
int quiescense(Board *board, int alpha, int beta, int d) {
    /*use<evalPosition>*/
    /*set<turn: turn$>*/
    int eval = evalPosition(board);
    
    quiescenceNodes ++;

    if(eval >= beta) return beta;
    if(eval > alpha) alpha = eval;

    Move moves[MG_MAX_MOVES];

    /*use<mg_gen>*/
    /*set<turn: turn$, genType: CAPTURES>*/
    int numMoves = mg_gen(board, moves);

    /*use<orderMoves>*/
    /*set<turn: turn$>*/
    orderMoves(board, moves, numMoves, MOVE_NONE);
    
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

        if(eval >= beta) 
            return beta;
        if(eval > alpha)
            alpha = eval;
    }
    
    return alpha;
}
/*endtemplate*/

/*template<turn: WHITE | BLACK>*/
int negamax(Board *board, int depth, int distFromRoot, int alpha, int beta) {
    
    Move ttMove = MOVE_NONE;
    int ttEval;

    if ((ttEval = tt_getEval(board->zobrist, alpha, beta, depth, &ttMove)) != TT_LOOKUP_FAILED)
        return ttEval;

    ttMove = MOVE_NONE;
    
    int evalType = ENTRY_ALPHA;
    Move bestMoveInPos = MOVE_NONE;

    searchNodes ++;
    if(depth == 0) {
        /*use<quiescense>*/
        /*set<turn: turn$>*/
        return quiescense(board, alpha, beta, 0);
    } else {
        Move moves[MG_MAX_MOVES];

        /*use<mg_gen>*/
        /*set<turn: turn$, genType: ALL>*/
        int numMoves = mg_gen(board, moves);

        /*use<orderMoves>*/
        /*set<turn: turn$>*/
        orderMoves(board, moves, numMoves, ttMove);
        if(board->mateStatus == CHECKMATE) {
            return -EVAL_INF + distFromRoot;
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
            int eval = -negamax(board, depth-1, distFromRoot + 1, -beta, -alpha);

            /*use<mv_unmake>*/
            /*set<turn: turn$>*/
            mv_unmake(&result, board);

            if(eval >= beta) {
                tt_storeEval(board->zobrist, eval, depth, ENTRY_BETA, moves[i]);
                return beta;
            }

            if(eval > alpha) {
                evalType = ENTRY_EXACT;
                bestMoveInPos = moves[i];
                alpha = eval;
            }
        }

        tt_storeEval(board->zobrist, alpha, depth, evalType, bestMoveInPos);

        return alpha;
    }
}
/*endtemplate*/

/*template<turn: WHITE | BLACK>*/
void searchRoot(Board *board, int depth) {
    insertionSort(rootMoves, rootMoveEvals, numRootMoves);

    int alpha = -EVAL_INF - 1;
    int beta = EVAL_INF + 1;

    int evalType = ENTRY_ALPHA;

    for(int i = 0; i < numRootMoves; i ++) {
        MoveResult result = MOVE_RESULT(rootMoves[i], board);

        /*use<mv_make>*/
        /*set<turn: turn$>*/
        mv_make(rootMoves[i], board);

        /*use<negamax>*/
        /*set<turn: turn$(WHITE -> BLACK | BLACK -> WHITE)>*/
        int eval = -negamax(board, depth-1, 1, -beta, -alpha);

        char str[LAN_MAX_SIZE];

        mv_toLAN(rootMoves[i], str);
        printf("%s: stored eval %d, calculated eval %d\n", str, rootMoveEvals[i], eval);

        /*use<mv_unmake>*/
        /*set<turn: turn$>*/
        mv_unmake(&result, board);
        rootMoveEvals[i] = eval;

        if (eval > alpha) {
            evalType = ENTRY_EXACT;
            bestMove = rootMoves[i];
            alpha = eval;
        }
    }

    tt_storeEval(board->zobrist, alpha, depth, evalType, bestMove);
}


/*template<turn: WHITE | BLACK>*/
Move findBestMove(Board *board, int depth) {
    searchNodes = 0;
    quiescenceNodes = 0;

    /*use<mg_gen>*/
    /*set<turn: turn$, genType: ALL>*/
    numRootMoves = mg_gen(board, rootMoves);

    /*use<evalMove>*/
    /*set<turn: turn$>*/
    for (int i = 0; i < numRootMoves; i ++)
        rootMoveEvals[i] = evalMove(board, rootMoves[i]);

    for (int d = 1; d <= 6; d ++) {
        printf("Searched depth %d\n", d);
        /*use<searchRoot>*/
        /*set<turn: turn$>*/
        searchRoot(board, d);
    }

    printf("%d %d\n", searchNodes, quiescenceNodes);

    return bestMove;
}
/*endtemplate*/