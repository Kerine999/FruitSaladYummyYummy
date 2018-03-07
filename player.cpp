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
vector<Move*> Player::possMoves(Board board, Side playSide)
{
    vector<Move*> poss;
    for(int i=0;i<8;i++)
    {
        for(int k=0;k<8;k++)
        {
            Move* curr=new Move(i,k);
            if(board.checkMove(curr,playSide))
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
 * Assigns a weight to a move given a particular board state and a side to check.
 */
int Player::getWeight(Move* move, Board* testBoard, Side playSide)
{
    Board* temp=testBoard->copy();
    int tempHeur = 0;
    if(playSide==BLACK)
    {
        tempHeur=temp->countBlack()-temp->countWhite();
    }
    else
    {
        tempHeur=temp->countWhite()-temp->countBlack();
    }
    if((move->getX()==0||move->getX()==7)&&(move->getY()==0||move->getY()==7))
    {
        if(tempHeur < 0)
            tempHeur/=40;
        else
            tempHeur*=40;
    }
    else if((move->getX()>=2 && move->getX()<=5)&&(move->getY()==0||move->getY()==7))
    {
        if(tempHeur < 0)
            tempHeur/=20;
        else
            tempHeur*=20;
    }
    else if((move->getX()==0||move->getX()==7)&&(move->getY()>=2&&move->getY()<=5))
    {
        if(tempHeur < 0)
            tempHeur/=20;
        else
            tempHeur*=20;
    }
    else if((move->getX()==1||move->getX()==6)&&(move->getY()==1||move->getY()==6))
    {
        if(tempHeur < 0)
            tempHeur*=40;
        else
            tempHeur/=40;
    }
    else if((move->getX()==0||move->getX()==7)&&(move->getY()==1||move->getY()==6))
    {
        if(tempHeur < 0)
            tempHeur*=20;
        else
            tempHeur/=20;
    }
    else if((move->getX()==1||move->getX()==6)&&(move->getY()==0||move->getY()==7))
    {
        if(tempHeur < 0)
            tempHeur*=20;
        else
            tempHeur/=20;
    }
    delete temp;

    return tempHeur;
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
    Move* m=make_move(side,0,currBoard);
    currBoard.doMove(m,side);
    return m;
}

/*
 * Perform a move using the simple heuristic, which is essentially a 2-ply mini_max
 * which applies the heuristic used in getWeight() rather than simply the
 * difference between stones.
 */
Move *Player::make_move(Side s, int depth, Board b)
{
    vector<Move*> poss = possMoves(b, s);
    double weight = -10000, tempWeight = 0;
    Move* finalMove = nullptr;
    if(poss.size() == 0)
    {
        return nullptr;
    }
    for(Move * move : poss)
    {
        Board *newBoard = b.copy();
        newBoard->doMove(move, s);
        if(depth<MAX_DEPTH)
        {
            if(s==BLACK)
            {
                Move* temp=make_move(WHITE,depth+1,*newBoard);
                if(temp!=nullptr)
                {
                    newBoard->doMove(temp, WHITE);
                }
            }
            else
            {
                Move* temp=make_move(BLACK,depth+1,*newBoard);
                if(temp!=nullptr)
                    newBoard->doMove(temp, BLACK);
            }
        }
        tempWeight = getWeight(move, newBoard, s);
        if(tempWeight > weight)
        {
            weight = tempWeight;
            finalMove = move;
        }
        delete newBoard;
    }
    Move *nextMove = new Move(finalMove->getX(), finalMove->getY());
    for(int i = 0; i < (int) poss.size(); i++)
    {
        delete poss[i];
    }
    return nextMove;
}

/*
 * Find the optimal move for the opponent to make in the minimax scheme, given
 * board state.
 */
Move *Player::findOppMove(Board board)
{
    vector<Move*> opponent_poss = possMoves(board, opponent);
    if(opponent_poss.size() == 0)
    {
        return nullptr;
    }
    Move *opp_move = nullptr;
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
    Move * opp = new Move(opp_move->getX(), opp_move->getY());
    for(int i = 0; i < (int) opponent_poss.size(); i++)
    {
        delete opponent_poss[i];
    }
    return opp;
}

/*
 * Perform a 2-ply mini_max algorithm to determine the next move to be made.
 */
Move *Player::mini_max()
{
    vector<Move*> poss = possMoves(currBoard, side);
    int pieceDiff = -100, tempDiff = 0;
    Move* finalMove = nullptr;
    if(poss.size() == 0)
    {
        return nullptr;
    }
    for(Move * move : poss)
    {
        Board *newBoard = currBoard.copy();
        newBoard->doMove(move, side);
        Move *opp_move = findOppMove(*newBoard);
        if(opp_move != nullptr)
        {
            newBoard->doMove(opp_move, opponent);
            delete opp_move;
        }
        if(side == BLACK)
        {
            tempDiff = newBoard->countBlack() - newBoard->countWhite();
        }
        else
        {
            tempDiff = newBoard->countWhite() - newBoard->countBlack();
        }
        if(pieceDiff < tempDiff)
        {
            pieceDiff = tempDiff;
            finalMove = move;
        }
        delete newBoard;
    }
    Move *nextMove = new Move(finalMove->getX(), finalMove->getY());
    for(int i = 0; i < (int) poss.size(); i++)
    {
        delete poss[i];
    }
    currBoard.doMove(nextMove, side);
    return nextMove;
}
