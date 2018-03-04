#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include <vector>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class State {

public:
    State(Board *board, Move* move)
    {
        this->board = board;
        this->move = move;
    }
    ~State()
    {
    }
    Board *board;
    Move* move;
};

class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    vector<Move*> possMoves(Board board, Side side);
    Move* best(vector<Move*> poss);
    Move* mini_max();
    Move *findOppMove(Board board);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax = true;
    Side opponent;
    Board currBoard;
    Side side;
    vector<State> states;
};

#endif
