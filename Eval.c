#include "Eval.h"

const int pawns[2][64] = {
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,  5,  0,  0,  5, -5,  5,
        -5, 0,  0, -20,-20, 0,  0, -5,
        0,  0,  0,  0,  0,  0,  0,  0
    },
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        -5, 0, 0,-20,-20, 0, 0,  -5,
        5, -5,  5,  0,  0,  5, -5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5,  5, 10, 25, 25, 10,  5,  5,
        10, 10, 20, 30, 30, 20, 10, 10,
        50, 50, 50, 50, 50, 50, 50, 50,
        0,  0,  0,  0,  0,  0,  0,  0,
    },
};

const int knights[2][64] = {
    {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 15, 15, 15,  5,-30,
        -30,  0, 15, 15, 15, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
    },
    {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
    }
};

const int bishops[2][64] = {
    {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-20,-10,-10,-20,-10,-20,
    }, 
    {
        -20,-10,-20,-10,-10,-20,-10,-20,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -20,-10,-10,-10,-10,-10,-10,-20,
    }
    
};

const int rooks[2][64] = {
    {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    },
    {
        0,  0,  0,  5,  5,  0,  0,  0
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        5, 10, 10, 10, 10, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0,
    }
    
};

const int queens[2][64] = {
    {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    },
    {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -10,  5,  5,  5,  5,  5,  0,-10,
        0,  0,  5,  5,  5,  5,  0, -5,
        -5,  0,  5,  5,  5,  5,  0, -5,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20,
    }
    
};

const int kings[2][64] = {
    {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        20, 20,  -10,  -10,  -10,  -10, 20, 20,
        20, 30, 10,  -10,  -10, 10, 30, 20
    },
    {
        20, 30, 10,  -10,  -10, 10, 30, 20,
        20, 20,  -10,  -10,  -10,  -10, 20, 20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
    }
};

const int pieceValues[5] = {PAWN_VALUE, KNIGHT_VALUE, BISHOP_VALUE, ROOK_VALUE, QUEEN_VALUE};

/*template<turn: WHITE | BLACK>*/
int evalMove(Board *board, Move move) {
    return 64 * MOVE_ORIGIN(move) + MOVE_DEST(move);

    int eval = 0;
    int dest =  MOVE_DEST(move);
    int origin =  MOVE_ORIGIN(move);

    /*if<turn: WHITE>*/
        #define OUR_TURN WHITE
        #define OPP_TURN BLACK
        #define OUR_PAWN WHITE_PAWN
    /*elif<turn: BLACK>*/
        #define OUR_TURN BLACK
        #define OPP_TURN WHITE
        #define OUR_PAWN BLACK_PAWN
    /*endif*/

    //penalize moving to square attacked by opponent pawn
    if(board->pieces[origin] != OUR_PAWN && (pawnCaptures[OUR_TURN][dest] & PAWNS(board, OPP_TURN)) != 0)
        eval -= pieceValues[PIECE_TYPE(board->pieces[origin])];

    //reward high-value captures with low-value pieces
    if(board->pieces[dest] != PIECE_NONE)
        eval += 10 * pieceValues[board->pieces[dest]] - pieceValues[board->pieces[origin]];

    //reward promotions
    if(MOVE_IS_PROMOTION(move)) {
        int special = MOVE_SPECIAL(move);
        if(special == KNIGHT_PROMOTION) {
            eval += KNIGHT_VALUE;
        } else if(special == BISHOP_PROMOTION) {
            eval += BISHOP_VALUE;
        } else if(special == ROOK_PROMOTION) {
            eval += ROOK_VALUE;
        } else if(special == QUEEN_PROMOTION) {
            eval += QUEEN_VALUE;
        }
    }

    #undef OUR_TURN
    #undef OPP_TURN
    #undef OUR_PAWN

    return eval;
}
/*endtemplate*/

/*template<turn: WHITE | BLACK>*/
int evalPosition(Board *board) {
    int eval = 0;
    Bitboard pieces = 0;

    for(int turn = 0; turn <= 1; turn ++) {
        int sideEval = 0;

        pieces = PAWNS(board, turn);
        while(pieces) {
            int square = BB_POP_LSB(pieces);
            sideEval += PAWN_VALUE + pawns[turn][square];
        }

        pieces = KNIGHTS(board, turn);
        while(pieces) {
            int square = BB_POP_LSB(pieces);
            sideEval += KNIGHT_VALUE + knights[turn][square];
        }

        pieces = BISHOPS(board, turn);
        while(pieces) {
            int square = BB_POP_LSB(pieces);
            sideEval += BISHOP_VALUE + bishops[turn][square];
        }

        pieces = ROOKS(board, turn);
        while(pieces) {
            int square = BB_POP_LSB(pieces);
            sideEval += ROOK_VALUE + rooks[turn][square];
        }

        pieces = QUEENS(board, turn);
        while(pieces) {
            int square = BB_POP_LSB(pieces);
            sideEval += QUEEN_VALUE + queens[turn][square];
        }

        sideEval += kings[turn][BB_GET_LSB(KINGS(board, turn))];

        eval += (turn == WHITE) ? sideEval : -sideEval;
    }


    /*if<turn: WHITE>*/
        return eval;
    /*elif<turn: BLACK>*/
        return -eval;
    /*endif*/
}
/*endtemplate*/