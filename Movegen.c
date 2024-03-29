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

#include "Movegen.h"

const uint64_t slidingMagics[64] = {
    0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL,
    0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL, 0xa000800080400034ULL, 0x4808020004000ULL,
    0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL,
    0x1002100004082ULL, 0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL,
    0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL, 0x800080204005ULL,
    0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL,
    0x9020010080800200ULL, 0x813241200148449ULL, 0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL,
    0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
    0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL,
    0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL, 0x280009023410300ULL, 0xe0100040002240ULL,
    0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL,
    0x2000009044210200ULL, 0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL,
    0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL
};

const uint64_t diagonalMagics[64] = {
    0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL,
    0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL, 0x400492088408100ULL, 0x201c401040c0084ULL,
    0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL,
    0x8144042209100900ULL, 0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL,
    0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL, 0x520040470104290ULL,
    0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL,
    0x188071040440a00ULL, 0x4800404002011c00ULL, 0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL,
    0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
    0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL,
    0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL, 0x111040120082000ULL, 0x6803040141280a00ULL,
    0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL,
    0x822088220820214ULL, 0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL,
    0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
};

const int slidingMHTBits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    11, 10, 10, 10, 10, 10, 10, 11,
    12, 11, 11, 11, 11, 11, 11, 12
};

const int diagonalMHTBits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6,
    5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 9, 9, 7, 5, 5,
    5, 5, 7, 7, 7, 7, 5, 5,
    5, 5, 5, 5, 5, 5, 5, 5,
    6, 5, 5, 5, 5, 5, 5, 6
};

const int knightJumps[8][2] = {
    {2, 1}, {1, 2}, {-2, 1}, {-1, 2}, {2, -1}, {1, -2}, {-2, -1}, {-1, -2}
};

const int directionOffsets[8][2] = {
    {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}
};

const int slidingDirections[4] = {
    NORTH, EAST, SOUTH, WEST
};

const int diagonalDirections[4] = {
    NORTHEAST, SOUTHEAST, SOUTHWEST, NORTHWEST
};

const Bitboard epRankMasks2[2] = {RANK_5_MASK, RANK_4_MASK};

Bitboard directionalMoves[64][8];

Bitboard knightMoves[64];
Bitboard pawnCaptures[2][64];
Bitboard kingMoves[64];
Bitboard diagonalMoves[64];
Bitboard neswMoves[64];
Bitboard slidingMoves[64];
Bitboard nsMoves[64];
Bitboard diagonalMovesWithLast[64];
Bitboard slidingMovesWithLast[64];
Bitboard pinRays[64][64];

MagicHashTable *slidingMHTs[64];
MagicHashTable *diagonalMHTs[64];
MagicHashTable *slidingPinMHTs[64];
MagicHashTable *diagonalPinMHTs[64];

void addMove(Bitboard *bitboard, int rank, int file) {
    if (rank >= 0 && rank < 8 && file >= 0 && file < 8)
        *bitboard |= (1ULL << SQUARE(rank, file));
} 

Bitboard genKnightMoves(int rank, int file) {
    Bitboard bitboard = 0;

    for (int i = 0; i < 8; i ++)
        addMove(&bitboard, rank + knightJumps[i][0], file + knightJumps[i][1]);
    
    return bitboard;
}

Bitboard genDoublePawnPushes(int turn, int rank, int file) {
    Bitboard bitboard = 0;

    if (rank == 1 && turn == WHITE)
        bitboard |= 1ULL << (24 + file);
    else if (rank == 6 && turn == BLACK)
        bitboard |= 1ULL << (32 + file);

    return bitboard;
}

Bitboard genSinglePawnPushes(int turn, int rank, int file) {
    Bitboard bitboard = 0;

    if (rank != 0 && rank != 7) {
        if (turn == WHITE) {
            bitboard |= 1ULL << SQUARE(rank + 1, file);
        } else if (turn == BLACK) {
            bitboard |= 1ULL << SQUARE(rank - 1, file);
        }
    }

    return bitboard;
}

Bitboard genPawnCaptures(int turn, int rank, int file) {
    Bitboard bitboard = 0;
    
    if (turn == WHITE) {
        addMove(&bitboard, rank + 1, file - 1);
        addMove(&bitboard, rank + 1, file + 1);
    } else if (turn == BLACK) {
        addMove(&bitboard, rank - 1, file - 1);
        addMove(&bitboard, rank - 1, file + 1);
    }

    return bitboard;
}

Bitboard genKingMoves(int rank, int file) {
    Bitboard bitboard = 0;

    for (int rankOff = -1; rankOff <= 1; rankOff ++) {
        for (int fileOff = -1; fileOff <= 1; fileOff ++) {
            if (rankOff != 0 || fileOff != 0) {
                addMove(&bitboard, rank + rankOff, file + fileOff);
            }
        }
    }

    return bitboard;
}

int inBounds(int rank, int file) {
    return (rank >= 0 && rank < 8 && file >= 0 && file < 8);
}

int notOnEdge(int rank, int file) {
    return (rank > 0 && rank < 7 && file > 0 && file < 7);
}

Bitboard genDirectionalMoves(int rank, int file, const int *directions, Bitboard blockers) {
    Bitboard moves = 0;
    
    for(int i = 0; i < 4; i ++) {   
        int dir = directions[i];   
           
        for(int dist = 1; dist < 8; dist ++) {
            int destRank = rank + directionOffsets[dir][0]*dist;
            int destFile = file + directionOffsets[dir][1]*dist;
            int destSquare = SQUARE(destRank, destFile);

            if(!inBounds(destRank, destFile)) break;

            moves |= 1ULL << destSquare;
            if(GET_BIT(blockers, destSquare)) break;
        }
    }

    return moves;
}

Bitboard genDirectionalPins(int rank, int file, const int *directions, Bitboard blockers) {
    Bitboard pins = 0;

    for (int i = 0; i < 4; i ++) {
        int dir = directions[i];
        int pinnedSquare = -1;

        for (int dist = 1; dist < 8; dist ++) {
            int newRank = rank + directionOffsets[dir][0] * dist;
            int newFile = file + directionOffsets[dir][1] * dist;
            int newSquare = SQUARE(newRank, newFile);

            if (inBounds(newRank, newFile) && GET_BIT(blockers, newSquare)) {
                if (pinnedSquare != -1) {
                    pinnedSquare = -1;
                    break;
                }

                pinnedSquare = newSquare;
            }
        }

        if (pinnedSquare != -1)
            pins |= (1ULL << pinnedSquare);
    }

    return pins;
}

void populateDirectionalMoves(int rank, int file, const int *directions, Bitboard *moves, Bitboard *movesWithLast, MagicHashTable *mht, MagicHashTable *pinmht) {
    int seenSquares[16];
    int numSeen = 0;

    for (int i = 0; i < 4; i ++) {
        Bitboard directionalMoves = 0;
        int dir = directions[i];

        for (int dist = 1; dist < 8; dist ++) {
            int destRank = rank + directionOffsets[dir][0]*dist;
            int destFile = file + directionOffsets[dir][1]*dist;
            int nextRank = destRank + directionOffsets[dir][0];
            int nextFile = destFile + directionOffsets[dir][1];

            int destSquare = SQUARE(destRank, destFile);

            if (inBounds(nextRank, nextFile)) {
                seenSquares[numSeen ++] = destSquare;
                directionalMoves |= 1ULL << destSquare;
                *moves |= 1ULL << destSquare;
            }

            if (inBounds(destRank, destFile)) {
                directionalMoves |= 1ULL << destSquare;
                *movesWithLast |= 1ULL << destSquare;
            }
        }

        if (dir == NORTH || dir == SOUTH) {
            nsMoves[SQUARE(rank, file)] |= directionalMoves;
        } else if (dir == NORTHEAST || dir == SOUTHWEST) {
            neswMoves[SQUARE(rank, file)] |= directionalMoves;
        }

    } 

    int numStates = 1 << numSeen;
    for (int state = 0; state < numStates; state ++) {
        Bitboard blockers = 0;
        for(int squareIdx = 0; squareIdx < numSeen; squareIdx ++) {
            blockers |= (Bitboard) GET_BIT(state, squareIdx) << seenSquares[squareIdx];
        }
        
        Bitboard legalMoves = genDirectionalMoves(rank, file, directions, blockers);
        mht_insert(mht, blockers, legalMoves);
        Bitboard pins = genDirectionalPins(rank, file, directions, blockers);
        mht_insert(pinmht, blockers, pins);
    }
}

void initPinRays() {
    for (int kingPos = 0; kingPos < 64; kingPos ++) {
        for (int attackPos = 0; attackPos < 64; attackPos ++) {
            Bitboard pinRay = 0;
            Bitboard fullRay = 0;

            int kingRank = RANK(kingPos);
            int kingFile = FILE(kingPos);
            int attackRank = RANK(attackPos);
            int attackFile = FILE(attackPos);

            int dRank = attackRank - kingRank;
            int dFile = attackFile - kingFile;

            int dist = MAX(abs(dRank), abs(dFile));

            if (dist != 0) {
                int rankDir = dRank / dist;
                int fileDir = dFile / dist;

                for (int i = 1; i <= dist; i ++) {
                    int rank = kingRank + i * rankDir;
                    int file = kingFile + i * fileDir;
                    int square = SQUARE(rank, file);

                    pinRay |= (1ULL << square);
                }

                int rank = kingRank;
                int file = kingFile;

                while (inBounds(rank, file)) {
                    fullRay |= (1ULL << SQUARE(rank, file));
                    rank += rankDir;
                    file += fileDir;
                }

                rank = kingRank;
                file = kingFile;

                while (inBounds(rank, file)) {
                    fullRay |= (1ULL << SQUARE(rank, file));
                    rank -= rankDir;
                    file -= fileDir;
                }

                pinRays[kingPos][attackPos] = pinRay;
            }
        }
    }
}

void mg_init(void) {
    initPinRays();

    for (int rank = 0; rank < 8; rank ++) {
        for (int file = 0; file < 8; file ++) {
            int square = rank * 8 + file;

            nsMoves[square] = 0;
            neswMoves[square] = 0;

            slidingMHTs[square] = mht_create(slidingMHTBits[square], slidingMagics[square]);
            slidingPinMHTs[square] = mht_create(slidingMHTBits[square], slidingMagics[square]);
            populateDirectionalMoves(rank, file, slidingDirections, &slidingMoves[square], &slidingMovesWithLast[square], slidingMHTs[square], slidingPinMHTs[square]);

            diagonalMHTs[square] = mht_create(diagonalMHTBits[square], diagonalMagics[square]);
            diagonalPinMHTs[square] = mht_create(diagonalMHTBits[square], diagonalMagics[square]);
            populateDirectionalMoves(rank, file, diagonalDirections, &diagonalMoves[square], &diagonalMovesWithLast[square], diagonalMHTs[square], diagonalPinMHTs[square]);

            knightMoves[square] = genKnightMoves(rank, file);
            kingMoves[square] = genKingMoves(rank, file);

            pawnCaptures[WHITE][square] = genPawnCaptures(WHITE, rank, file);
            pawnCaptures[BLACK][square] = genPawnCaptures(BLACK, rank, file);
        }
    }
}

/*template<turn: WHITE | BLACK, genType: CAPTURES | ALL>*/
int mg_gen(Board *board, Move *moves) {
    /*if<genType: ALL>*/
    board->mateStatus = NO_MATE;
    /*endif*/

    int turn = TURN(board);
    int oppTurn = 1 - turn;
    int movec = 0;

    /*if<genType: CAPTURES>*/
    Bitboard captureMask = board->colorBitboards[oppTurn];
    /*elif<genType: ALL>*/
    Bitboard captureMask = BB_FULL_BOARD;
    /*endif*/

    Bitboard king = KINGS(board, turn);
    Bitboard withoutKing = board->bitboard ^ king;
    int kingPos = BB_GET_LSB(king);    

    //Generate attacked squares
    Bitboard attackedSquares = 0;
    
    //Add pawn attacks
    Bitboard bb = PAWNS(board, oppTurn);
    /*if<turn: WHITE>*/
        attackedSquares |= (bb & NOT_FILE_8_MASK) >> 7;
        attackedSquares |= (bb & NOT_FILE_1_MASK) >> 9;
    /*elif<turn: BLACK>*/
        attackedSquares |= (bb & NOT_FILE_1_MASK) << 7;
        attackedSquares |= (bb & NOT_FILE_8_MASK) << 9;
    /*endif*/

    //Add knight attacks
    bb = KNIGHTS(board, oppTurn);
    while (bb) {
        int square = BB_POP_LSB(bb);
        attackedSquares |= knightMoves[square];
    }
    
    Bitboard oppSliders = ROOKS(board, oppTurn) | QUEENS(board, oppTurn);
    Bitboard oppDiagonals = BISHOPS(board, oppTurn) | QUEENS(board, oppTurn);

    //Add sliding attacks
    bb = oppSliders;
    while (bb) {
        int square = BB_POP_LSB(bb);
        Bitboard blockers = slidingMoves[square] & withoutKing;
        attackedSquares |= MHT_GET_VALUE(slidingMHTs[square], blockers);
    }

    //Add diagonal attacks
    bb = oppDiagonals;
    while (bb) {
        int square = BB_POP_LSB(bb);
        Bitboard blockers = diagonalMoves[square] & withoutKing;
        attackedSquares |= MHT_GET_VALUE(diagonalMHTs[square], blockers);
    }

    //Add king attacks
    attackedSquares |= kingMoves[BB_GET_LSB(KINGS(board, oppTurn))];

    //Generate checkers
    Bitboard checkers = knightMoves[kingPos] & KNIGHTS(board, oppTurn); //Add knight checkers
    checkers |= pawnCaptures[turn][kingPos] & PAWNS(board, oppTurn); //Add pawn checkers
    checkers |= MHT_GET_VALUE(diagonalMHTs[kingPos], diagonalMoves[kingPos] & board->bitboard) & oppDiagonals; //Add diagonal checkers
    checkers |= MHT_GET_VALUE(slidingMHTs[kingPos], slidingMoves[kingPos] & board->bitboard) & oppSliders; //Add sliding checkers
    
    //Generate legal king moves - the king cannot move onto an attacked square or capture its own color piece
    bb = kingMoves[kingPos] & ~attackedSquares & ~board->colorBitboards[turn] & captureMask;
    while (bb) {
        int dest = BB_POP_LSB(bb);
        moves[movec ++] = CREATE_MOVE(kingPos, dest, QUIET);
    }

    int numCheckers = BB_POP_COUNT(checkers);

    //If we are in double check, the only legal moves are the king moves just generated
    if (numCheckers == 2) {
        /*if<genType: ALL>*/
        if (movec == 0) 
            board->mateStatus = CHECKMATE;
        /*endif*/

        return movec;
    }
    
    //Generate pinned pieces

    //X-ray sliding attackers (attackers ignoring our color pieces)
    bb = oppSliders & MHT_GET_VALUE(slidingMHTs[kingPos], slidingMoves[kingPos] & board->colorBitboards[oppTurn]);

    //Add rays from each sliding attacker to the king
    Bitboard slidingPins = 0;
    while (bb) {
        int square = BB_POP_LSB(bb);
        slidingPins |= pinRays[kingPos][square];
    }
    
    //Exclude pins with more than one protector
    slidingPins = MHT_GET_VALUE(slidingPinMHTs[kingPos], slidingPins & board->colorBitboards[turn]);

    //Split sliding pins into vertical and horizontal directions
    Bitboard nsPins = slidingPins & nsMoves[kingPos];
    Bitboard ewPins = slidingPins ^ nsPins;

    //Same, but with diagonal attackers
    bb = oppDiagonals & MHT_GET_VALUE(diagonalMHTs[kingPos], diagonalMoves[kingPos] & board->colorBitboards[oppTurn]);

    Bitboard diagonalPins = 0;
    while (bb) {
        int square = BB_POP_LSB(bb);
        diagonalPins |= pinRays[kingPos][square];
    }

    diagonalPins = MHT_GET_VALUE(diagonalPinMHTs[kingPos], diagonalPins & board->colorBitboards[turn]);

    Bitboard neswPins = diagonalPins & neswMoves[kingPos];
    Bitboard nwsePins = diagonalPins ^ neswPins;

    //The move mask consists of all of the legal moves that stop a check (full board if the king is not in check) AND fit the desired move generation type
    //This does not include check evasions, which have been generated previously
    Bitboard moveMask = captureMask;
    Bitboard checkMask = BB_FULL_BOARD;
    
    if(numCheckers == 1) {
        if (checkers & (oppSliders | oppDiagonals)) {
            //If we are attacked by a sliding or diagonal piece, the move mask is a ray from the king to the piece, inclusive
            checkMask = pinRays[kingPos][BB_GET_LSB(checkers)];
        } else {
            //If we are attacked by any other piece, we can only capture it
            checkMask = checkers;
        }
    }

    moveMask &= checkMask;

    Bitboard emptySquares = ~board->bitboard;
    Bitboard notOurColor = ~board->colorBitboards[turn];

    //No pawn can move if it is pinned horizontally
    Bitboard pawns = PAWNS(board, turn) & ~ewPins;
    int epFile = EP_FILE(board);

    /*if<turn: WHITE>*/
        Bitboard pawnsOn7 = pawns & RANK_7_MASK;
        pawns ^= pawnsOn7;

        //Pawns not on file 1 can left capture a black piece if they are not pinned vertically or ne-sw diagonally
        Bitboard bb1 = ((pawns & NOT_FILE_1_MASK & ~(nsPins | neswPins)) << 7) & board->colorBitboards[BLACK] & moveMask;
        //Pawns not on file 8 can right capture a black piece if they are not pinned verticaly or nw-se diagonally
        Bitboard bb2 = ((pawns & NOT_FILE_8_MASK & ~(nsPins | nwsePins)) << 9) & board->colorBitboards[BLACK] & moveMask;

        //Left pawn captures
        while (bb1) {
            int dest = BB_POP_LSB(bb1);
            moves[movec ++] = CREATE_MOVE(dest - 7, dest, QUIET);
        }

        //Right pawn captures
        while (bb2) {
            int dest = BB_POP_LSB(bb2);
            moves[movec ++] = CREATE_MOVE(dest - 9, dest, QUIET);
        }

        //Pawns can only be pushed to empty squares if they are not pinned diagonally
        bb1 = ((pawns & ~(neswPins | nwsePins)) << 8) & emptySquares;
        //Pawns can only be double pushed to empty squares in the fourth rank if they can be single pushed
        bb2 = (bb1 << 8) & emptySquares & RANK_4_MASK & moveMask;
        bb1 &= moveMask;
        
        //Double pawn pushes
        while (bb2) {
            int dest = BB_POP_LSB(bb2);
            moves[movec ++] = CREATE_MOVE(dest - 16, dest, DOUBLE_PAWN_PUSH);
        }

        //Single pawn pushes
        while (bb1) {
            int dest = BB_POP_LSB(bb1);
            moves[movec ++] = CREATE_MOVE(dest - 8, dest, QUIET);
        }

        //Only generate promotions if there are pawns able to promote
        if (pawnsOn7) {
            //Same logic as above, but only for pawns on rank 7
            bb1 = ((pawnsOn7 & NOT_FILE_1_MASK & ~(nsPins | neswPins)) << 7) & board->colorBitboards[BLACK] & moveMask;
            bb2 = ((pawnsOn7 & NOT_FILE_8_MASK & ~(nsPins | nwsePins)) << 9) & board->colorBitboards[BLACK] & moveMask;

            //Left capture promotions
            while (bb1) {
                int dest = BB_POP_LSB(bb1);
                CREATE_PROMOTIONS(moves, movec, dest - 7, dest);
            }

            //Right capture promotions
            while (bb2) {
                int dest = BB_POP_LSB(bb2);
                CREATE_PROMOTIONS(moves, movec, dest - 9, dest);
            }

            bb1 = ((pawnsOn7 & ~(neswPins | nwsePins)) << 8) & ~board->bitboard & moveMask;

            //Push promotions            
            while (bb1) {
                int dest = BB_POP_LSB(bb1);
                CREATE_PROMOTIONS(moves, movec, dest - 8, dest);
            }
            
        }

        //En passant captures
        if (epFile >= 0) {
            //Generate the destination square for ep captures
            Bitboard epDest = ((SQUARE_32 << epFile) & checkMask) << 8;

            //Pawns not on file 1 can left ep capture if they are not pinned vertically or ne-sw diagonally
            Bitboard lEpDest = ((pawns & NOT_FILE_1_MASK & ~(nsPins | neswPins)) << 7) & epDest;
            //Pawns not on file 8 can right ep capture if they are not pinned vertically or nw-se diagonally
            Bitboard rEpDest = ((pawns & NOT_FILE_8_MASK & ~(nsPins | nwsePins)) << 9) & epDest;

            if (lEpDest) {
                int dest = BB_GET_LSB(lEpDest);
                int origin = dest - 7;
                
                //Simulate ep capture
                Bitboard afterEp = board->bitboard ^ ((1ULL << origin) | (1ULL << (dest - 8))) ^ lEpDest;

                //Find discovered checkers, if any (note that the only discovered checkers here can be sliders)
                Bitboard discoveredCheckers = (PIECE_BITBOARD(board, BLACK_QUEEN) | PIECE_BITBOARD(board, BLACK_ROOK)) & MHT_GET_VALUE(slidingMHTs[kingPos], slidingMoves[kingPos] & afterEp);

                //Ensure the king will not be in check after taking ep
                if(discoveredCheckers == 0)
                    moves[movec ++] = CREATE_MOVE(origin, dest, EP_CAPTURE);
            }

            if (rEpDest) {
                int dest = BB_GET_LSB(rEpDest);
                int origin = dest - 9;
                
                Bitboard afterEp = board->bitboard ^ ((1ULL << origin) | (1ULL << (dest - 8))) ^ rEpDest;

                Bitboard discoveredCheckers = (PIECE_BITBOARD(board, BLACK_QUEEN) | PIECE_BITBOARD(board, BLACK_ROOK)) & MHT_GET_VALUE(slidingMHTs[kingPos], slidingMoves[kingPos] & afterEp);

                if(discoveredCheckers == 0)
                    moves[movec ++] = CREATE_MOVE(origin, dest, EP_CAPTURE);
            }
        }
    /*elif<turn: BLACK>*/
        Bitboard pawnsOn2 = pawns & RANK_2_MASK;
        pawns ^= pawnsOn2;

        Bitboard singlePushes = ((pawns & ~(neswPins | nwsePins)) >> 8) & emptySquares;
        Bitboard doublePushes = (singlePushes >> 8) & emptySquares & RANK_5_MASK & moveMask;
        singlePushes &= moveMask;

        Bitboard lCaptures = ((pawns & NOT_FILE_8_MASK & ~(nsPins | neswPins)) >> 7) & board->colorBitboards[WHITE] & moveMask;
        Bitboard rCaptures = ((pawns & NOT_FILE_1_MASK & ~(nsPins | nwsePins)) >> 9) & board->colorBitboards[WHITE] & moveMask;

        while (lCaptures) {
            int dest = BB_POP_LSB(lCaptures);
            moves[movec ++] = CREATE_MOVE(dest + 7, dest, QUIET);
        }

        while (rCaptures) {
            int dest = BB_POP_LSB(rCaptures);
            moves[movec ++] = CREATE_MOVE(dest + 9, dest, QUIET);
        }

        while (doublePushes) {
            int dest = BB_POP_LSB(doublePushes);
            moves[movec ++] = CREATE_MOVE(dest + 16, dest, DOUBLE_PAWN_PUSH);
        }

        while (singlePushes) {
            int dest = BB_POP_LSB(singlePushes);
            moves[movec ++] = CREATE_MOVE(dest + 8, dest, QUIET);
        }

        if (pawnsOn2) {
            Bitboard pushPromotions = ((pawnsOn2 & ~(neswPins | nwsePins)) >> 8) & emptySquares & moveMask;
            Bitboard lPromotions = ((pawnsOn2 & NOT_FILE_8_MASK & ~(nsPins | neswPins)) >> 7) & board->colorBitboards[WHITE] & moveMask;
            Bitboard rPromotions = ((pawnsOn2 & NOT_FILE_1_MASK & ~(nsPins | nwsePins)) >> 9) & board->colorBitboards[WHITE] & moveMask;

            while (pushPromotions) {
                int dest = BB_POP_LSB(pushPromotions);
                CREATE_PROMOTIONS(moves, movec, dest + 8, dest);
            }

            while (lPromotions) {
                int dest = BB_POP_LSB(lPromotions);
                CREATE_PROMOTIONS(moves, movec, dest + 7, dest);
            }

            while (rPromotions) {
                int dest = BB_POP_LSB(rPromotions);
                CREATE_PROMOTIONS(moves, movec, dest + 9, dest);
            }
        }

        if (epFile >= 0) {
            Bitboard epDest = ((SQUARE_24 << epFile) & checkMask) >> 8;

            Bitboard lEpDest = ((pawns & NOT_FILE_8_MASK & ~(nsPins | neswPins)) >> 7) & epDest;
            Bitboard rEpDest = ((pawns & NOT_FILE_1_MASK & ~(nsPins | nwsePins)) >> 9) & epDest;

            if (lEpDest) {
                int dest = BB_GET_LSB(lEpDest);
                int origin = dest + 7;
                
                Bitboard afterEp = board->bitboard ^ ((1ULL << origin) | (1ULL << (dest + 8))) ^ lEpDest;

                //Find discovered checkers, if any
                Bitboard discoveredCheckers = (PIECE_BITBOARD(board, WHITE_QUEEN) | PIECE_BITBOARD(board, WHITE_ROOK)) & MHT_GET_VALUE(slidingMHTs[kingPos], slidingMoves[kingPos] & afterEp);

                //Ensure the king will not be in check after taking ep
                if(discoveredCheckers == 0)
                    moves[movec ++] = CREATE_MOVE(origin, dest, EP_CAPTURE);
            }

            if (rEpDest) {
                int dest = BB_GET_LSB(rEpDest);
                int origin = dest + 9;
                
                Bitboard afterEp = board->bitboard ^ ((1ULL << origin) | (1ULL << (dest + 8))) ^ rEpDest;

                //Find discovered checkers, if any
                Bitboard discoveredCheckers = (PIECE_BITBOARD(board, WHITE_QUEEN) | PIECE_BITBOARD(board, WHITE_ROOK)) & MHT_GET_VALUE(slidingMHTs[kingPos], slidingMoves[kingPos] & afterEp);

                //Ensure the king will not be in check after taking ep
                if(discoveredCheckers == 0)
                    moves[movec ++] = CREATE_MOVE(origin, dest, EP_CAPTURE);
            }
        }
    /*endif*/

    Bitboard allPins = slidingPins | diagonalPins;

    //Generate knight moves
    Bitboard knights = KNIGHTS(board, turn) & ~allPins;
    while (knights) {
        int origin = BB_POP_LSB(knights);
        Bitboard moveBitboard = knightMoves[origin] & notOurColor & moveMask;

        while (moveBitboard) {
            int dest = BB_POP_LSB(moveBitboard);
            moves[movec ++] = CREATE_MOVE(origin, dest, QUIET); 
        }
    }

    Bitboard sliders = ROOKS(board, turn) | QUEENS(board, turn);
    //Note that diagonally pinned sliders cannot move
    Bitboard pinnedSliders = sliders & slidingPins;
    Bitboard unpinnedSliders = sliders & ~allPins;

    //Generate moves for pinned sliders
    while (pinnedSliders) {
        int origin = BB_POP_LSB(pinnedSliders);
        Bitboard blockers = slidingMoves[origin] & board->bitboard;

        //Pinned sliders can only move along the pin ray
        Bitboard moveBitboard = MHT_GET_VALUE(slidingMHTs[origin], blockers) & notOurColor & moveMask & slidingMovesWithLast[kingPos];

        while (moveBitboard) {
            int dest = BB_POP_LSB(moveBitboard);
            moves[movec ++] = CREATE_MOVE(origin, dest, QUIET);
        }
    }

    //Generate moves for unpinned sliders
    while (unpinnedSliders) {
        int origin = BB_POP_LSB(unpinnedSliders);
        Bitboard blockers = slidingMoves[origin] & board->bitboard;
        Bitboard moveBitboard = MHT_GET_VALUE(slidingMHTs[origin], blockers) & notOurColor & moveMask;

        while (moveBitboard) {
            int dest = BB_POP_LSB(moveBitboard);
            moves[movec ++] = CREATE_MOVE(origin, dest, QUIET);
        }
    }
    
    //Same as above, but for diagonals    
    Bitboard diagonals = BISHOPS(board, turn) | QUEENS(board, turn);
    Bitboard pinnedDiagonals = diagonals & diagonalPins;
    Bitboard unpinnedDiagonals = diagonals & ~allPins;

    while (pinnedDiagonals) {
        int origin = BB_POP_LSB(pinnedDiagonals);
        Bitboard blockers = diagonalMoves[origin] & board->bitboard;
        Bitboard moveBitboard = MHT_GET_VALUE(diagonalMHTs[origin], blockers) & notOurColor & moveMask & diagonalMovesWithLast[kingPos];

        while (moveBitboard) {
            int dest = BB_POP_LSB(moveBitboard);
            moves[movec ++] = CREATE_MOVE(origin, dest, QUIET);
        }
    }

    while (unpinnedDiagonals) {
        int origin = BB_POP_LSB(unpinnedDiagonals);
        Bitboard blockers = diagonalMoves[origin] & board->bitboard;
        Bitboard moveBitboard = MHT_GET_VALUE(diagonalMHTs[origin], blockers) & notOurColor & moveMask;

        while (moveBitboard) {
            int dest = BB_POP_LSB(moveBitboard);
            moves[movec ++] = CREATE_MOVE(origin, dest, QUIET);
        }
    }

    /*if<genType: ALL>*/
    //We cannot castle out of check
    if(numCheckers == 0) {
        //We cannot castle through squares that are either attacked or occupied
        bb = attackedSquares | board->bitboard;
        if(turn == WHITE) {
            if(HAS_CASTLE_RIGHT(board, WHITE_CASTLE_KINGSIDE_RIGHT) && (bb & WHITE_KINGSIDE_CASTLE_MASK) == 0)
                moves[movec++] = CREATE_MOVE(4, 6, KINGSIDE_CASTLE);
            //For queenside castling, we must also check that the rook's path is not blocked
            if(HAS_CASTLE_RIGHT(board, WHITE_CASTLE_QUEENSIDE_RIGHT) && ((bb & WHITE_QUEENSIDE_CASTLE_MASK) | (board->bitboard & (1ULL << 1))) == 0)
                moves[movec++] = CREATE_MOVE(4, 2, QUEENSIDE_CASTLE);
        } else if(turn == BLACK) {
            if(HAS_CASTLE_RIGHT(board, BLACK_CASTLE_KINGSIDE_RIGHT) && (bb & BLACK_KINGSIDE_CASTLE_MASK) == 0)
                moves[movec++] = CREATE_MOVE(60, 62, KINGSIDE_CASTLE);
            if(HAS_CASTLE_RIGHT(board, BLACK_CASTLE_QUEENSIDE_RIGHT) && ((bb & BLACK_QUEENSIDE_CASTLE_MASK) | (board->bitboard & (1ULL << 57))) == 0)
                moves[movec++] = CREATE_MOVE(60, 58, QUEENSIDE_CASTLE);
        }
        
        if (movec == 0)
            board->mateStatus = STALEMATE;
    } else if (movec == 0) {
        board->mateStatus = CHECKMATE;
    }
    /*endif*/

    return movec;
}
/*endtemplate*/
