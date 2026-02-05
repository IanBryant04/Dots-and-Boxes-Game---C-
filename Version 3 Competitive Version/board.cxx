#include <iostream>
#include <assert.h>
#include "common.h"
#include "board.h"

using namespace std;

void Board::AllocateBoard(int dots_in_rows, int dots_in_cols, int& blanklinecount)
{
    assert(board == nullptr);
    rows = dots_in_rows * 2 - 1;
    cols = dots_in_cols * 2 - 1;

    board = new char* [rows];
    for(int r = 0; r < rows; r++)
        board[r] = new char[cols];

    blanklinecount = 0;
    for(int r = 0; r < rows; r ++)
        for(int c = 0; c < cols; c ++)
        {
            board[r][c] = ' ';
            if(Loc(r, c).IsLineLocation())
                blanklinecount++;
        }

    for(int r = 0; r < rows; r += 2)
        for(int c = 0; c < cols; c += 2)
            board[r][c] = '.';
}

void Board::FreeBoard()
{
    if(board != nullptr)
    {
        for(int r = 0; r < rows; r++)
            delete[] board[r];
        delete[] board;
        board = nullptr;
    }
}

ostream& operator << (ostream& os, const Board& board)
{
    cout << "   ";
    for(int i=0; i<board.cols; i++)
    {
        if(i % 10 == 0) cout << (i/10);
        else            cout << ' ';
    }
    cout << endl;
    cout << "   ";
    for(int i=0; i<board.cols; i++)
        cout << (i%10);
    cout << endl;

    for(int r=0; r<board.GetRows(); r++)
    {
        if(r%10 == 0) cout << (r/10) << (r%10) << ' ';
        else          cout << ' '    << (r%10) << ' ';

        for(int c=0; c<board.GetCols(); c++)
        {
            Loc loc(r,c);
            char b_rc = board(r,c);
            if(b_rc == ' ') cout << b_rc;
            else if(loc.IsLineVerticalLocation  () && b_rc != ' ') cout << "|";
            else if(loc.IsLineHorizontalLocation() && b_rc != ' ') cout << "-";
            else cout << b_rc;
        }
        cout << endl;
    }
    return os;
}
