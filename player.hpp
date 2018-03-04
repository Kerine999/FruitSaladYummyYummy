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
    vector<Move*> possMoves();
    Move* best(vector<Move*> poss);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
    Side opponent;
    Board currBoard;
    Side side;
};

#endif
