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
Bitboard singlePawnPushes[2][64];
Bitboard doublePawnPushes[2][64];
Bitboard pawnCaptures[2][64];
Bitboard fileMasks[8];

Bitboard kingMoves[64];
Bitboard diagonalMoves[64];
Bitboard slidingMoves[64];
Bitboard diagonalMovesWithLast[64];
Bitboard slidingMovesWithLast[64];
Bitboard pinRays[64][64];
Bitboard fullRays[64][64];

MagicHashTable *slidingMHTs[64];
MagicHashTable *diagonalMHTs[64];
MagicHashTable *slidingPinMHTs[64];
MagicHashTable *diagonalPinMHTs[64];

static inline int popCount(uint64_t a) {
    return __builtin_popcountll(a);
}

static inline int getLSB(uint64_t *a) {
    return __builtin_ctzll(*a);
}

static inline int popLSB(uint64_t *a) {
    int index = getLSB(a);
    *a &= *a - 1;
    return index;
}

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

void printBitboard(Bitboard *bitboard) {
    for(int rank = 7; rank >= 0; rank --) {
        for(int file = 0; file < 8; file ++) {
            int i = rank*8 + file;
            if(((*bitboard >> i) & 1) == 1) printf("# ");
            else printf(". ");
        }
        printf("\n");
    }
    printf("\n");
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
        int dir = directions[i];

        for (int dist = 1; dist < 7; dist ++) {
            int destRank = rank + directionOffsets[dir][0]*dist;
            int destFile = file + directionOffsets[dir][1]*dist;
            int nextRank = destRank + directionOffsets[dir][0];
            int nextFile = destFile + directionOffsets[dir][1];

            int destSquare = SQUARE(destRank, destFile);

            if (inBounds(nextRank, nextFile)) {
                seenSquares[numSeen ++] = destSquare;
                *moves |= 1ULL << destSquare;
            }

            if (inBounds(destRank, destFile)) {
                *movesWithLast |= 1ULL << destSquare;
            }
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

                fullRays[kingPos][attackPos] = fullRay;
                pinRays[kingPos][attackPos] = pinRay;
            }
        }
    }
}

void mg_init(void) {
    initPinRays();

    for (int file = 0; file < 8; file ++)
        fileMasks[file] = 0x0101010101010101ULL << file;

    for (int rank = 0; rank < 8; rank ++) {
        for (int file = 0; file < 8; file ++) {
            int square = rank * 8 + file;

            slidingMHTs[square] = mht_create(slidingMHTBits[square], slidingMagics[square]);
            slidingPinMHTs[square] = mht_create(slidingMHTBits[square], slidingMagics[square]);
            populateDirectionalMoves(rank, file, slidingDirections, &slidingMoves[square], &slidingMovesWithLast[square], slidingMHTs[square], slidingPinMHTs[square]);

            diagonalMHTs[square] = mht_create(diagonalMHTBits[square], diagonalMagics[square]);
            diagonalPinMHTs[square] = mht_create(diagonalMHTBits[square], diagonalMagics[square]);
            populateDirectionalMoves(rank, file, diagonalDirections, &diagonalMoves[square], &diagonalMovesWithLast[square], diagonalMHTs[square], diagonalPinMHTs[square]);

            knightMoves[square] = genKnightMoves(rank, file);
            kingMoves[square] = genKingMoves(rank, file);

            for (int turn = WHITE; turn <= BLACK; turn ++) {
                singlePawnPushes[turn][square] = genSinglePawnPushes(turn, rank, file);
                doublePawnPushes[turn][square] = genDoublePawnPushes(turn, rank, file);
                pawnCaptures[turn][square] = genPawnCaptures(turn, rank, file);
            }
        }
    }
}

int mg_genPseudoLegal(Board *board, Move *moves) {
    int movec = 0;
    int turn = TURN(board);

    Bitboard knights = KNIGHTS(board, turn);
    while (knights) {
        int origin = popLSB(&knights);
        Bitboard moveBitboard = knightMoves[origin] & ~board->colorBitboards[turn];

        while (moveBitboard)
            moves[movec ++] = CREATE_MOVE(origin, popLSB(&moveBitboard), QUIET); 
    }
    
    Bitboard pawns = PAWNS(board, turn);
    int epFile = EP_FILE(board);

    if (turn == WHITE) {
        Bitboard pawnsOn7 = pawns & RANK_7_MASK;
        pawns ^= pawnsOn7;

        Bitboard singlePushes = (pawns << 8) & ~board->bitboard;
        Bitboard doublePushes = (singlePushes << 8) & ~board->bitboard & 0x00000000FF000000ULL;
        Bitboard lCaptures = ((pawns & ~fileMasks[0]) << 7) & board->colorBitboards[BLACK];
        Bitboard rCaptures = ((pawns & ~fileMasks[7]) << 9) & board->colorBitboards[BLACK];

        Bitboard pushPromotions = (pawnsOn7 << 8) & ~board->bitboard;
        Bitboard lPromotions = ((pawnsOn7 & ~fileMasks[0]) << 7) & board->colorBitboards[BLACK];
        Bitboard rPromotions = ((pawnsOn7 & ~fileMasks[7]) << 9) & board->colorBitboards[BLACK];

        while (singlePushes) {
            int dest = popLSB(&singlePushes);
            moves[movec ++] = CREATE_MOVE(dest - 8, dest, QUIET);
        }

        while (doublePushes) {
            int dest = popLSB(&doublePushes);
            moves[movec ++] = CREATE_MOVE(dest - 16, dest, DOUBLE_PAWN_PUSH);
        }

        while (lCaptures) {
            int dest = popLSB(&lCaptures);
            moves[movec ++] = CREATE_MOVE(dest - 7, dest, QUIET);
        }

        while (rCaptures) {
            int dest = popLSB(&rCaptures);
            moves[movec ++] = CREATE_MOVE(dest - 9, dest, QUIET);
        }

        while (pushPromotions) {
            int dest = popLSB(&pushPromotions);

            moves[movec ++] = CREATE_MOVE(dest - 8, dest, QUEEN_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest - 8, dest, ROOK_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest - 8, dest, BISHOP_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest - 8, dest, KNIGHT_PROMOTION);
        }

        while (lPromotions) {
            int dest = popLSB(&lPromotions);

            moves[movec ++] = CREATE_MOVE(dest - 7, dest, QUEEN_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest - 7, dest, ROOK_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest - 7, dest, BISHOP_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest - 7, dest, KNIGHT_PROMOTION);
        }

        while (rPromotions) {
            int dest = popLSB(&rPromotions);

            moves[movec ++] = CREATE_MOVE(dest - 9, dest, QUEEN_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest - 9, dest, ROOK_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest - 9, dest, BISHOP_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest - 9, dest, KNIGHT_PROMOTION);
        }

        if (epFile >= 0) {
            Bitboard epPawn = PAWNS(board, BLACK) & fileMasks[EP_FILE(board)] & RANK_5_MASK;
            int epDest = __builtin_ctzll(epPawn << 8);
            Bitboard epOrigins = (((epPawn & ~fileMasks[0]) >> 1) | ((epPawn & ~fileMasks[7]) << 1)) & PAWNS(board, WHITE);

            while (epOrigins)
                moves[movec ++] = CREATE_MOVE(popLSB(&epOrigins), epDest, EP_CAPTURE);
        }
    } else if (turn == BLACK) {
        Bitboard pawnsOn2 = pawns & RANK_2_MASK;
        pawns ^= pawnsOn2;

        Bitboard singlePushes = (pawns >> 8) & ~board->bitboard;
        Bitboard doublePushes = (singlePushes >> 8) & ~board->bitboard & 0x000000FF00000000ULL;
        Bitboard lCaptures = ((pawns & ~fileMasks[7]) >> 7) & board->colorBitboards[WHITE];
        Bitboard rCaptures = ((pawns & ~fileMasks[0]) >> 9) & board->colorBitboards[WHITE];
        
        Bitboard pushPromotions = (pawnsOn2 >> 8) & ~board->bitboard;
        Bitboard lPromotions = ((pawnsOn2 & ~fileMasks[7]) >> 7) & board->colorBitboards[WHITE];
        Bitboard rPromotions = ((pawnsOn2 & ~fileMasks[0]) >> 9) & board->colorBitboards[WHITE];

        while (singlePushes) {
            int dest = popLSB(&singlePushes);
            moves[movec ++] = CREATE_MOVE(dest + 8, dest, QUIET);
        }

        while (doublePushes) {
            int dest = popLSB(&doublePushes);
            moves[movec ++] = CREATE_MOVE(dest + 16, dest, DOUBLE_PAWN_PUSH);
        }

        while (lCaptures) {
            int dest = popLSB(&lCaptures);
            moves[movec ++] = CREATE_MOVE(dest + 7, dest, QUIET);
        }

        while (rCaptures) {
            int dest = popLSB(&rCaptures);
            moves[movec ++] = CREATE_MOVE(dest + 9, dest, QUIET);
        }

        while (pushPromotions) {
            int dest = popLSB(&pushPromotions);

            moves[movec ++] = CREATE_MOVE(dest + 8, dest, QUEEN_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest + 8, dest, ROOK_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest + 8, dest, BISHOP_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest + 8, dest, KNIGHT_PROMOTION);
        }

        while (lPromotions) {
            int dest = popLSB(&lPromotions);

            moves[movec ++] = CREATE_MOVE(dest + 7, dest, QUEEN_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest + 7, dest, ROOK_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest + 7, dest, BISHOP_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest + 7, dest, KNIGHT_PROMOTION);
        }

        while (rPromotions) {
            int dest = popLSB(&rPromotions);

            moves[movec ++] = CREATE_MOVE(dest + 9, dest, QUEEN_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest + 9, dest, ROOK_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest + 9, dest, BISHOP_PROMOTION);
            moves[movec ++] = CREATE_MOVE(dest + 9, dest, KNIGHT_PROMOTION);
        }

        if (epFile >= 0) {
            Bitboard epPawn = PAWNS(board, WHITE) & fileMasks[EP_FILE(board)] & RANK_4_MASK;
            int epDest = __builtin_ctzll(epPawn >> 8);
            Bitboard epOrigins = (((epPawn & ~fileMasks[0]) >> 1) | ((epPawn & ~fileMasks[7]) << 1)) & PAWNS(board, BLACK);

            while (epOrigins)
                moves[movec ++] = CREATE_MOVE(popLSB(&epOrigins), epDest, EP_CAPTURE);
        }
    }

    Bitboard sliders = ROOKS(board, turn) | QUEENS(board, turn);
    while (sliders) {
        int origin = popLSB(&sliders);
        Bitboard blockers = slidingMoves[origin] & board->bitboard;
        Bitboard moveBitboard = MHT_GET_VALUE(slidingMHTs[origin], blockers) & ~board->colorBitboards[turn];

        while (moveBitboard)
            moves[movec ++] = CREATE_MOVE(origin, popLSB(&moveBitboard), QUIET);
    }
    
    Bitboard diagonals = BISHOPS(board, turn) | QUEENS(board, turn);
    while (diagonals) {
        int origin = popLSB(&diagonals);
        Bitboard blockers = diagonalMoves[origin] & board->bitboard;
        Bitboard moveBitboard = MHT_GET_VALUE(diagonalMHTs[origin], blockers) & ~board->colorBitboards[turn];

        while (moveBitboard)
            moves[movec ++] = CREATE_MOVE(origin, popLSB(&moveBitboard), QUIET);
    }

    return movec;
}

//maybe handle king moves in here instead of in pseudo legal

int mg_gen(Board *board, Move *moves) {
    int turn = TURN(board);
    int oppTurn = 1 - turn;

    int movec = 0;
    Bitboard king = KINGS(board, turn);
    int kingPos = getLSB(&king);    

    Bitboard attackedSquares = 0;
    Bitboard withoutKing = board->bitboard ^ king;
    
    Bitboard oppPawns = PAWNS(board, oppTurn);
    if (turn == WHITE) {
        attackedSquares |= (oppPawns & NOT_FILE_8_MASK) >> 7;
        attackedSquares |= (oppPawns & NOT_FILE_1_MASK) >> 9;
    } else if (turn == BLACK) {
        attackedSquares |= (oppPawns & NOT_FILE_1_MASK) << 7;
        attackedSquares |= (oppPawns & NOT_FILE_8_MASK) << 9;
    }

    Bitboard oppKnights = KNIGHTS(board, oppTurn);
    while (oppKnights)
        attackedSquares |= knightMoves[popLSB(&oppKnights)];
    
    Bitboard oppSliders = ROOKS(board, oppTurn) | QUEENS(board, oppTurn);
    Bitboard slidingAttacks = 0;
    while (oppSliders) {
        int square = popLSB(&oppSliders);
        Bitboard blockers = slidingMoves[square] & withoutKing;
        slidingAttacks |= MHT_GET_VALUE(slidingMHTs[square], blockers);
    }

    oppSliders = ROOKS(board, oppTurn) | QUEENS(board, oppTurn);

    Bitboard oppDiagonals = BISHOPS(board, oppTurn) | QUEENS(board, oppTurn);
    Bitboard diagonalAttacks = 0;
    while (oppDiagonals) {
        int square = popLSB(&oppDiagonals);
        Bitboard blockers = diagonalMoves[square] & withoutKing;
        diagonalAttacks |= MHT_GET_VALUE(diagonalMHTs[square], blockers);
    }

    oppDiagonals = BISHOPS(board, oppTurn) | QUEENS(board, oppTurn);

    attackedSquares |= (slidingAttacks | diagonalAttacks);
    attackedSquares |= kingMoves[getLSB(&KINGS(board, oppTurn))];

    Bitboard checkers = knightMoves[kingPos] & KNIGHTS(board, oppTurn);
    checkers |= pawnCaptures[turn][kingPos] & PAWNS(board, oppTurn);
    Bitboard diagonalBlockers = diagonalMoves[kingPos] & board->bitboard;
    checkers |= MHT_GET_VALUE(diagonalMHTs[kingPos], diagonalBlockers) & oppDiagonals;
    Bitboard slidingBlockers = slidingMoves[kingPos] & board->bitboard;
    checkers |= MHT_GET_VALUE(slidingMHTs[kingPos], slidingBlockers) & oppSliders;
    
    int numCheckers = popCount(checkers);

    if (numCheckers == 2) {
        Bitboard legalMoves = kingMoves[kingPos] & ~attackedSquares & ~board->colorBitboards[turn];
        while (legalMoves) {
            int dest = popLSB(&legalMoves);
            moves[movec ++] = CREATE_MOVE(kingPos, dest, QUIET);
        }

        return movec;
    }
    

    Move pseudoLegalMoves[MG_MAX_MOVES];
    int numPseudoLegal = mg_genPseudoLegal(board, pseudoLegalMoves);
    
    Bitboard slidingPinnedPieces = 0;
    Bitboard xRaySlidingAttackers = oppSliders & MHT_GET_VALUE(slidingMHTs[kingPos], slidingMoves[kingPos] & board->colorBitboards[oppTurn]);
    while (xRaySlidingAttackers)
        slidingPinnedPieces |= pinRays[kingPos][popLSB(&xRaySlidingAttackers)];
    
    slidingPinnedPieces = MHT_GET_VALUE(slidingPinMHTs[kingPos], slidingPinnedPieces & board->colorBitboards[turn]);

    Bitboard diagonalPinnedPieces = 0;
    Bitboard xRayDiagonalAttackers = oppDiagonals & MHT_GET_VALUE(diagonalMHTs[kingPos], diagonalMoves[kingPos] & board->colorBitboards[oppTurn]);

    while (xRayDiagonalAttackers)
        diagonalPinnedPieces |= pinRays[kingPos][popLSB(&xRayDiagonalAttackers)];

    diagonalPinnedPieces = MHT_GET_VALUE(diagonalPinMHTs[kingPos], diagonalPinnedPieces & board->colorBitboards[turn]);
    
    Bitboard pinnedPieces = slidingPinnedPieces | diagonalPinnedPieces;

    if(numCheckers == 1) {
        Bitboard rayChecker = checkers & (ROOKS(board, oppTurn) | BISHOPS(board, oppTurn) | QUEENS(board, oppTurn));
        Bitboard legalMoves = (kingMoves[kingPos] & ~attackedSquares & ~board->colorBitboards[turn]);

        if (rayChecker)
            legalMoves &= ~fullRays[kingPos][getLSB(&rayChecker)] | rayChecker;
        
        while (legalMoves) {
            int dest = popLSB(&legalMoves);
            moves[movec ++] = CREATE_MOVE(kingPos, dest, QUIET);
        }
               
        legalMoves = checkers & (PAWNS(board, oppTurn) | KNIGHTS(board, oppTurn));
        
        if (rayChecker) {
            int checkerPos = getLSB(&rayChecker);
            legalMoves = pinRays[kingPos][checkerPos];
        }

        for (int i = 0; i < numPseudoLegal; i ++) {
            Move move = pseudoLegalMoves[i];
            int origin = MOVE_ORIGIN(move);
            int dest = MOVE_DEST(move);
            int special = MOVE_SPECIAL(move);

            if (special == EP_CAPTURE) {
                int epFile = EP_FILE(board);
                Bitboard epPawn = fileMasks[epFile] & epRankMasks2[turn];

                if (epPawn & checkers) {
                    Bitboard blockers = board->bitboard ^ ((1ULL << origin) | (1ULL << dest)) ^ epPawn;

                    Bitboard discoveredCheckers = oppDiagonals & MHT_GET_VALUE(diagonalMHTs[kingPos], diagonalMoves[kingPos] & blockers);
                    discoveredCheckers |= oppSliders & MHT_GET_VALUE(slidingMHTs[kingPos], slidingMoves[kingPos] & blockers);

                    if(discoveredCheckers == 0)
                        moves[movec ++] = move;
                }
            } else {
                Bitboard pinRay = fullRays[kingPos][origin];

                if (GET_BIT(legalMoves, dest) && ((GET_BIT(pinnedPieces, origin) ^ 1) | GET_BIT(pinRay, dest)))
                    moves[movec ++] = move;
            }
        }
        
        return movec;
    }

    for (int i = 0; i < numPseudoLegal; i ++) {
        int legal = 1;
        Move move = pseudoLegalMoves[i];
        int origin = MOVE_ORIGIN(move);
        int dest = MOVE_DEST(move);
        int special = MOVE_SPECIAL(move);

        if (special == EP_CAPTURE) {
            int epFile = EP_FILE(board);
            Bitboard epPawn = fileMasks[epFile] & epRankMasks2[turn];
            Bitboard blockers = board->bitboard ^ ((1ULL << origin) | (1ULL << dest)) ^ epPawn;

            Bitboard discoveredCheckers = oppDiagonals & MHT_GET_VALUE(diagonalMHTs[kingPos], diagonalMoves[kingPos] & blockers);
            discoveredCheckers |= oppSliders & MHT_GET_VALUE(slidingMHTs[kingPos], slidingMoves[kingPos] & blockers);

            legal = (discoveredCheckers == 0);
        } else {
            Bitboard pinRay = fullRays[kingPos][origin];
            legal &= ~GET_BIT(pinnedPieces, origin) | GET_BIT(pinRay, dest);
        }

        if (legal)
            moves[movec ++] = move;
    }

    //generate king moves
    Bitboard legalMoves = kingMoves[kingPos] & ~attackedSquares & ~board->colorBitboards[turn];
    while (legalMoves) {
        int dest = popLSB(&legalMoves);
        moves[movec ++] = CREATE_MOVE(kingPos, dest, QUIET);
    }

    if(numCheckers == 0) {
        uint64_t castleCheck = attackedSquares | board->bitboard;
        if(turn == WHITE) {
            if(HAS_CASTLE_RIGHT(board, WHITE_CASTLE_KINGSIDE_RIGHT) && (castleCheck & WHITE_KINGSIDE_CASTLE_MASK) == 0)
                moves[movec++] = CREATE_MOVE(4, 6, KINGSIDE_CASTLE);
            if(HAS_CASTLE_RIGHT(board, WHITE_CASTLE_QUEENSIDE_RIGHT) && ((castleCheck & WHITE_QUEENSIDE_CASTLE_MASK) | (board->bitboard & (1ULL << 1))) == 0)
                moves[movec++] = CREATE_MOVE(4, 2, QUEENSIDE_CASTLE);
        } else if(turn == BLACK) {
            if(HAS_CASTLE_RIGHT(board, BLACK_CASTLE_KINGSIDE_RIGHT) && (castleCheck & BLACK_KINGSIDE_CASTLE_MASK) == 0)
                moves[movec++] = CREATE_MOVE(60, 62, KINGSIDE_CASTLE);
            if(HAS_CASTLE_RIGHT(board, BLACK_CASTLE_QUEENSIDE_RIGHT) && ((castleCheck & BLACK_QUEENSIDE_CASTLE_MASK) | (board->bitboard & (1ULL << 57))) == 0)
                moves[movec++] = CREATE_MOVE(60, 58, QUEENSIDE_CASTLE);
        }
    }

    return movec;
}