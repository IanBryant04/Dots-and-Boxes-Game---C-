#ifndef __PLAYER__
#define __PLAYER__

#include <iostream>
#include "common.h"
#include "board.h"
using namespace std;

class IPlayer
{
public:
    // return the player information that includes full name and email address,
    //  such that "Micky Mouse (mm123@psu.edu)"
    virtual string PlayerInfo() = 0;
    // Init(const int,const int) will be called before playing the game
    // You can create your own data-structure
    virtual void Init
        ( int  board_rows  // the size of board (including dots, lines, and boxes)
        , int  board_cols  // the size of board (including dots, lines, and boxes)
        , char box_type    // the character for the player's boxes
        , char line_type   // the character for the player's lines
        ) = 0;
    // Close() will be called after finishing playing the game
    // You can remove all dynamically allocated memories
    virtual void Close() = 0;
    // EventAddLine() and EventAddBox() will be called
    // when a player adds a line or when a system assign a box's owner
    virtual void EventAddLine(char bar, const Loc& loc) = 0;
    virtual void EventAddBox (char box, const Loc& loc) = 0;
    // Loc SelectLineLocation() will be called
    // when the game system ask where your player want to add a line
    virtual Loc SelectLineLocation() = 0;
};

#endif
