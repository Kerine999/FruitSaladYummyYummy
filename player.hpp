#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.hpp"
#include "board.hpp"
using namespace std;


class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    vector<Move*> possMoves(Board board, Side side);
    int getWeight(Move* move, Board* testBoard, Side playSide);
    Move *make_move(Side s, int depth, Board b);
    Move *findOppMove1(Board board);
    Move* mini_max();
    Move *findOppMove(Board board);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side opponent;
    Board currBoard;
    Side side;
    int MAX_DEPTH=2;
};

#endif
