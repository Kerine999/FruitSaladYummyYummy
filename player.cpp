#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    this->side=side;
    if(side==BLACK)
    {
        opponent=WHITE;
    }
    else
    {
        opponent=BLACK;
    }
    currBoard=Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Returns a list of the possible moves for your side.
 */
vector<Move*> Player::possMoves(Board board, Side side)
{
    vector<Move*> poss;
    for(int i=0;i<8;i++)
    {
        for(int k=0;k<8;k++)
        {
            Move* curr=new Move(i,k);
            if(board.checkMove(curr,side))
            {
                poss.push_back(curr);
            }
            else
            {
                delete curr;
            }
        }
    }
    return poss;
}

/**
 * Returns the best move from a list of moves using the heuristic
 * number of our pieces-number of opponent pieces. *5 multiplier for corner moves,
 * *3 multiplier to edge moves not adjacent to corner, /5 multiplier for move diagonally
 * adjacent to corner, /3 multiplier for edge moves adjacent to corner.
 */
Move* Player::best(vector<Move*> poss)
{
    int bestIndex=0;
    int best=-100;
    for (int i=0;i<(int)poss.size();i++)
    {
        Board* temp=currBoard.copy();
        temp->doMove(poss[i],side);
        int tempHeur;
        if(side==BLACK)
        {
            tempHeur=temp->countBlack()-temp->countWhite();
        }
        else
        {
            tempHeur=temp->countWhite()-temp->countBlack();
        }
        if((poss[i]->getX()==0||poss[i]->getX()==7)&&(poss[i]->getY()==0||poss[i]->getY()==7))
        {
            tempHeur*=5;
        }
        else if((poss[i]->getX()>=2 && poss[i]->getX()<=5)&&(poss[i]->getY()==0||poss[i]->getY()==7))
        {
            tempHeur*=3;
        }
        else if((poss[i]->getX()==0||poss[i]->getX()==7)&&(poss[i]->getY()>=2&&poss[i]->getY()<=5))
        {
            tempHeur*=3;
        }
        else if((poss[i]->getX()==1||poss[i]->getX()==6)&&(poss[i]->getY()==1||poss[i]->getY()==6))
        {
            tempHeur/=5;
        }
        else if((poss[i]->getX()==0||poss[i]->getX()==7)&&(poss[i]->getY()==1||poss[i]->getY()==6))
        {
            tempHeur/=3;
        }
        else if((poss[i]->getX()==1||poss[i]->getX()==6)&&(poss[i]->getY()==0||poss[i]->getY()==7))
        {
            tempHeur/=3;
        }
        if(tempHeur>best)
        {
            best=tempHeur;
            bestIndex=i;
        }
        delete temp;
    }
    return poss[bestIndex];
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    currBoard.doMove(opponentsMove, opponent);
    if(testingMinimax)
    {
        return mini_max();
    }
    vector<Move*> poss=possMoves(currBoard, side);
    if(poss.size() == 0)
    {
        return nullptr;
    }
    Move* move=best(poss);
    currBoard.doMove(move, side);
    for(int i=0;i<(int)poss.size();i++)
    {
        if(poss[i]!=move)
        {
            delete poss[i];
        }
    }
    return move;
}

Move *Player::findOppMove(Board board)
{
    vector<Move*> opponent_poss = possMoves(board, opponent);
    Move *opp_move;
    int tempDiff, pieces = -100;
    for(Move * move : opponent_poss)
    {
        Board *temp = board.copy();
        temp->doMove(move, opponent);
        if(opponent == BLACK)
        {
            tempDiff = temp->countBlack() - temp->countWhite();
        }
        else
        {
            tempDiff = temp->countWhite() - temp->countBlack();
        }
        if(pieces < tempDiff)
        {
            pieces = tempDiff;
            opp_move = move;
        }
        delete temp;
    }
    for(int i = 0; i < (int)opponent_poss.size(); i++)
    {
        if(opponent_poss[i] != opp_move)
        {
            delete opponent_poss[i];
        }
    }
    return opp_move;
}

Move *Player::mini_max()
{
    vector<Move*> poss = possMoves(currBoard, side);
    if(poss.size() == 0)
    {
        return nullptr;
    }
    for(Move * move : poss)
    {
        Board *newBoard = currBoard.copy();
        newBoard->doMove(move, side);
        State state = State(newBoard, move);
        states.push_back(state);
    }
    int pieceDiff = -100, tempDiff = 0;
    Move* finalMove = nullptr;
    for(State state : states)
    {
        Move *opp_move = findOppMove(*(state.board));
        state.board->doMove(opp_move, opponent);
        if(side == BLACK)
        {
            tempDiff = state.board->countBlack() - state.board->countWhite();
        }
        else
        {
            tempDiff = state.board->countWhite() - state.board->countBlack();
        }
        if(pieceDiff < tempDiff)
        {
            pieceDiff = tempDiff;
            finalMove = state.move;
        }
        delete opp_move;
    }
    for(int i = 0; i < (int)poss.size(); i++)
    {
        if(poss[i] != finalMove)
        {
            delete poss[i];
        }
    }
    //states.clear();
    return finalMove;
}
