#include <iostream>
#include <assert.h>
#include <cstdlib> // for srand and rand
#include <ctime>   // for time

#include "common.h"
#include "player.h"
#include "random_player.h"

using namespace std;

extern "C" IPlayer* PlayerFactory()
{
    return new RandomPlayer();
}

RandomPlayer::RandomPlayer()
{
    srand(time(0));
}

RandomPlayer::~RandomPlayer()
{
}

void RandomPlayer::Init(int _dots_in_rows, int _dots_in_cols, char _player_box, char _player_line)
{
    board.AllocateBoard(_dots_in_rows, _dots_in_cols);
    player_box  = _player_box ;
    player_line = _player_line;
    emptylines = new Loc[board.GetRows() * board.GetCols()];
}

void RandomPlayer::Close()
{
    board.FreeBoard();
    int emptylines_cnt = board.GetRows() * board.GetCols();
    delete[] emptylines;
}

void RandomPlayer::EventAddLine(const char bar, const Loc& loc)
{
    assert(loc.IsLineLocation());
    assert(board(loc) == ' ');
    board(loc) = bar;
}

void RandomPlayer::EventAddBox(const char box, const Loc& loc)
{
    assert(loc.IsBoxLocation());
    assert(board(loc) == ' ');
    board(loc) = box;
}

Loc RandomPlayer::SelectLineLocation()
{
    ListEmptyLines();
    int randloc = rand() % emptylines_cnt;
    return emptylines[randloc];
}

void RandomPlayer::ListEmptyLines()
{
    emptylines_cnt = 0;
    for(int row=0; row<board.GetRows(); row++)
    {
        for(int col=0; col<board.GetCols(); col++)
        {
            if((row % 2 == 0) && (col % 2 == 0)) continue; // dot
            if((row % 2 == 1) && (col % 2 == 1)) continue; // box
            if(board(row, col) == ' ')
            {
                emptylines[emptylines_cnt].row = row;
                emptylines[emptylines_cnt].col = col;
                emptylines_cnt ++;
            }
        }
    }
}
