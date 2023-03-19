"""
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
"""

import subprocess
import os

def setup_position(cur_moves, position):
    cmd = 'position fen ' + position + ' moves'

    for move in cur_moves:
        cmd += ' ' + move
    
    cmd += "\n"
    return cmd

def carlsim_perft(depth, cur_moves, position):
    input = 'isready\n' + setup_position(cur_moves, position) + '\nperft ' + str(depth) + '\nquit\n'

    p = subprocess.run(['../carlsim.out'], input=input, capture_output=True, text=True)

    lines = p.stdout.splitlines()
    result = lines[-2].split(" ")[2]
    split = lines[1:-2]

    moves = []

    for line in split:
        moves.append(line.split(": "))

    return (moves, result)

def stockfish_perft(depth, cur_moves, position):
    input = setup_position(cur_moves, position) + '\ngo perft ' + str(depth) + '\nquit\n'

    p = subprocess.run(['./stockfish'], input=input, capture_output=True, text=True)
    
    lines = p.stdout.splitlines()

    result = lines[-2].split(" ")[2]
    split = lines[1:-3]

    moves = []

    for line in split:
        moves.append(line.split(": "))

    return (moves, result)

def comparePerft(depth, cur_moves, position):
    (carlsim_moves, carlsim_result) = carlsim_perft(depth, cur_moves, position)
    (stockfish_moves, stockfish_result) = stockfish_perft(depth, cur_moves, position)

    if carlsim_result == stockfish_result:
        return None

    for stockfish_move in stockfish_moves:
        for carlsim_move in carlsim_moves:
            if stockfish_move[0] == carlsim_move[0] and stockfish_move[1] != carlsim_move[1]:
                return stockfish_move[0]
            
    return None

def testPosition(position, depth):
    cur_moves = []

    passed = False

    if depth == 1:
        carlsim_result = carlsim_perft(depth, cur_moves, position)[1]
        stockfish_result = stockfish_perft(depth, cur_moves, position)[1]

        passed = (carlsim_result == stockfish_result)
    else:
        for i in range(depth, 1, -1):
            move = comparePerft(i, cur_moves, position)

            if i == depth and move == None:
                passed = True
                break
            
            cur_moves.append(move)
            print(move)
        
    if not passed:
        p = subprocess.run(['./stockfish'], input=setup_position(cur_moves, position) + '\nd\nquit\n', capture_output=True, text=True)
        print(p.stdout)

    return passed

'''
testPositions = [
    ['rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ', 7],
    ['r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1', 6],
    ['8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1', 8],
    ['r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1', 6],
    ['r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ', 6],
    ['rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8', 5],
    ['r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10', 6],
    ['r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2', 1],
    ['8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3', 1],
    ['r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2', 1],
    ['r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2', 1],
    ['2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2', 1],
    ['rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9', 1],
    ['2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4', 1],
    ['rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8', 3],
    ['r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10', 3],
    ['3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1', 6],
    ['8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1', 6],
    ['8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1', 6],
    ['5k2/8/8/8/8/8/8/4K2R w K - 0 1', 6],
    ['3k4/8/8/8/8/8/8/R3K3 w Q - 0 1', 6],
    ['r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1', 4],
    ['r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1', 4],
    ['2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1', 6],
    ['8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1', 5],
    ['4k3/1P6/8/8/8/8/K7/8 w - - 0 1', 6],
    ['8/P1k5/K7/8/8/8/8/8 w - - 0 1', 6],
    ['K1k5/8/P7/8/8/8/8/8 w - - 0 1', 6],
    ['8/k1P5/8/1K6/8/8/8/8 w - - 0 1', 7],
    ['8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1', 4],
]
'''

testPositions = [
    ['rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ', 6],
    ['r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1', 5],
    ['8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1', 7],
    ['r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1', 5],
    ['r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1 ', 5],
    ['rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8', 5],
    ['r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10', 5],
    ['r6r/1b2k1bq/8/8/7B/8/8/R3K2R b KQ - 3 2', 1],
    ['8/8/8/2k5/2pP4/8/B7/4K3 b - d3 0 3', 1],
    ['r1bqkbnr/pppppppp/n7/8/8/P7/1PPPPPPP/RNBQKBNR w KQkq - 2 2', 1],
    ['r3k2r/p1pp1pb1/bn2Qnp1/2qPN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQkq - 3 2', 1],
    ['2kr3r/p1ppqpb1/bn2Qnp1/3PN3/1p2P3/2N5/PPPBBPPP/R3K2R b KQ - 3 2', 1],
    ['rnb2k1r/pp1Pbppp/2p5/q7/2B5/8/PPPQNnPP/RNB1K2R w KQ - 3 9', 1],
    ['2r5/3pk3/8/2P5/8/2K5/8/8 w - - 5 4', 1],
    ['rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8', 3],
    ['r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10', 3],
    ['3k4/3p4/8/K1P4r/8/8/8/8 b - - 0 1', 6],
    ['8/8/4k3/8/2p5/8/B2P2K1/8 w - - 0 1', 6],
    ['8/8/1k6/2b5/2pP4/8/5K2/8 b - d3 0 1', 6],
    ['5k2/8/8/8/8/8/8/4K2R w K - 0 1', 6],
    ['3k4/8/8/8/8/8/8/R3K3 w Q - 0 1', 6],
    ['r3k2r/1b4bq/8/8/8/8/7B/R3K2R w KQkq - 0 1', 4],
    ['r3k2r/8/3Q4/8/8/5q2/8/R3K2R b KQkq - 0 1', 4],
    ['2K2r2/4P3/8/8/8/8/8/3k4 w - - 0 1', 6],
    ['8/8/1P2K3/8/2n5/1q6/8/5k2 b - - 0 1', 5],
    ['4k3/1P6/8/8/8/8/K7/8 w - - 0 1', 6],
    ['8/P1k5/K7/8/8/8/8/8 w - - 0 1', 6],
    ['K1k5/8/P7/8/8/8/8/8 w - - 0 1', 6],
    ['8/k1P5/8/1K6/8/8/8/8 w - - 0 1', 7],
    ['8/8/2k5/5q2/5n2/8/5K2/8 b - - 0 1', 4],
]

print("Building...")
os.chdir(os.path.join(os.path.abspath(os.path.curdir),u'..'))
p = subprocess.run(['./build.sh'], capture_output=True)
os.chdir(os.path.join(os.path.abspath(os.path.curdir),u'Python/'))
print(p.stderr)

for position in testPositions:
    for depth in range(1, position[1] + 1):
        if testPosition(position[0], depth):
            print("PASS: depth: " + str(depth) + ", position: " + position[0])
        else:
            print("FAIL: depth: " + str(depth) + ", position: " + position[0])
            break