
#ifndef STRATEGIC_PLAYER_H
#define STRATEGIC_PLAYER_H

#include "board.h"

class StrategicPlayer {
private:
    //check how many lines surround a box not visible outside class
    int CountBoxSides(const Board& board, int r_dot, int c_dot) const;

public:
    //Selects the best available idea
    void SelectLineLocation(const Board& board,int& r, int& c);
};

#endif