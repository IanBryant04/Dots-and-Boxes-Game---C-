#ifndef RANDOM_PLAYER_H
#define RANDOM_PLAYER_H

#include "board.h"

class RandomPlayer {
public:
    //Selects a random valid line from the board dont think we need more funcs
    void SelectLineLocation(const Board& board, int& r, int& c);
};

#endif