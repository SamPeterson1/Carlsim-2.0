#!/bin/bash
python3 Python/TemplatePreprocessor.py Main.c Movegen.c Movegen.h Board.c Board.h Fen.c Fen.h Piece.c Piece.h StrUtil.c StrUtil.h Log.c Log.h Move.c Move.h Perft.c Perft.h MagicHashTable.c MagicHashTable.h Eval.c Eval.h Search.c Search.h uci.c uci.h Zobrist.c Zobrist.h Tests.h TranspositionTable.c TranspositionTable.h
cd Processed/
gcc -g -pg -g3 -O3 -Wall -o ../carlsim.out Main.c Movegen.c Movegen.h Board.c Board.h Fen.c Fen.h Piece.c Piece.h StrUtil.c StrUtil.h Log.c Log.h Move.c Move.h Perft.c Perft.h uci.c uci.h MagicHashTable.c MagicHashTable.h Eval.c Eval.h Search.c Search.h Zobrist.c Zobrist.h Tests.h TranspositionTable.c TranspositionTable.h
