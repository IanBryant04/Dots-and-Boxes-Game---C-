#ifndef __RANDOM_PLAYER__
#define __RANDOM_PLAYER__

#include <iostream>
#include "common.h"
#include "player.h"
#include "board.h"
using namespace std;

class RandomPlayer : public IPlayer
{
private:
    Board board;
    char player_box;
    char player_line;
    Loc* emptylines;
    int  emptylines_cnt;
public:
    RandomPlayer();
    ~RandomPlayer();
    string PlayerInfo() { return "random player"; }
    void Init(int dots_in_rows, int dots_in_cols, char player_box, char player_line);
    // Close() will be called after finishing playing the game
    // You can remove all dynamically allocated memories
    void Close();
    void EventAddLine(char bar, const Loc& loc);
    void EventAddBox(char box, const Loc& loc);
    Loc SelectLineLocation();
    void ListEmptyLines();
};

#endif
