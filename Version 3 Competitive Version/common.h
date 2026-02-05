#ifndef __COMMON__
#define __COMMON__

#include <iostream>
#include <string>
using namespace std;

class Loc
{
public:
    int row, col;
    Loc()                                 { row = -1;      col = -1;      }
    Loc(const Loc& loc)                   { row = loc.row; col = loc.col; }
    Loc(int nr, int nc)                   { row = nr;      col = nc;      }
    bool IsDotLocation() const            { return ((row % 2 == 0) && (col % 2 == 0)); }
    bool IsBoxLocation() const            { return ((row % 2 == 1) && (col % 2 == 1)); }
    bool IsLineHorizontalLocation() const { return ((row % 2 == 0) && (col % 2 == 1)); }
    bool IsLineVerticalLocation() const   { return ((row % 2 == 1) && (col % 2 == 0)); }
    bool IsLineLocation() const           { return IsLineHorizontalLocation() || IsLineVerticalLocation(); }
};

#endif
