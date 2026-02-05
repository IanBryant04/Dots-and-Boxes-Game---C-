//---------------------------------------------------------------------
// Name: Ian Bryant
// Email: iab5219@psu.edu
// Class: CMPSC 330
// Assignment: Homework 5
// Due Date: December 5, 2025
// Description: Strategic player implementation, uses a smarter strategy to win
// with safe moves/sacrificial moves when necessary.
//acknowledgement:
//1.had to look up use of extern since not covered https://www.freecodecamp.org/news/extern-keyword-function-tutorial/
//2.did not work with other students for this or the last homework
//3.used bash wiki for when code wasnt running https://mywiki.wooledge.org/BashGuide



#include <iostream>
#include <cstdlib>
#include <ctime>
#include "common.h"
#include "player.h"
#include "board.h"

using namespace std;

class MyStrategicPlayer : public IPlayer {
private:
    Board board;
    char player_box;
    char player_line;
    Loc* emptylines;
    int emptylines_cnt;

    void ListEmptyLines();
    int CountBoxSides(int r_dot, int c_dot) const;

public:
    MyStrategicPlayer();
    ~MyStrategicPlayer();
    string PlayerInfo();
    void Init(int board_rows, int board_cols, char box_type, char line_type);
    void Close();
    void EventAddLine(char bar, const Loc& loc);
    void EventAddBox(char box, const Loc& loc);
    Loc SelectLineLocation();
};


extern "C" IPlayer* PlayerFactory() {
    return new MyStrategicPlayer();
}

//initialize random seed
MyStrategicPlayer::MyStrategicPlayer() {
    srand(time(0)); //used to replicate ouput
}

MyStrategicPlayer::~MyStrategicPlayer() {
}

string MyStrategicPlayer::PlayerInfo() {
    return "Ian Bryant (iab5219@psu.edu)";
}

//Initialize board
void MyStrategicPlayer::Init(int board_rows, int board_cols, char box_type, char line_type) {
    board.AllocateBoard(board_rows, board_cols);
    player_box = box_type;
    player_line = line_type;
    //gets board size
    emptylines = new Loc[board.GetRows() * board.GetCols()];
}

//cleanup
void MyStrategicPlayer::Close() {
    board.FreeBoard();
    delete[] emptylines;
}

//used for EITHER player
void MyStrategicPlayer::EventAddLine(char bar, const Loc& loc) {
    board(loc)=bar;
}

//updates boxes for players
void MyStrategicPlayer::EventAddBox(char box, const Loc& loc) {
    board(loc)=box;
}

//searches for empty lines
void MyStrategicPlayer::ListEmptyLines() {
    emptylines_cnt=0;
    for(int row = 0;row < board.GetRows(); row++) {
        for(int col = 0; col < board.GetCols(); col++) {
            Loc loc(row, col);
            if(loc.IsLineLocation() && board(loc) == ' ') {
                emptylines[emptylines_cnt] = loc;
                emptylines_cnt++;
            }
        }
    }
}


//Returns 0-4 for valid boxes, 5 = PROBLEM
int MyStrategicPlayer::CountBoxSides(int r_dot, int c_dot) const {
    if (r_dot < 0||c_dot < 0 ||
        (r_dot + 2)>= board.GetRows() ||
        (c_dot + 2)>= board.GetCols()) {
        return 5; //out of bounds
    }

    //check if both are even
    if (r_dot % 2 != 0 || c_dot % 2 != 0) {
        return 5; //not a dot location
    }

    int count = 0;

    //top
    if (board(r_dot, c_dot + 1) != ' ')
        count++;

    //bottom
    if (board(r_dot + 2, c_dot + 1) != ' ')
        count++;

    //left
    if (board(r_dot + 1, c_dot) != ' ')
        count++;

    //right
    if (board(r_dot + 1, c_dot + 2) != ' ')
        count++;

    return count;
}

Loc MyStrategicPlayer::SelectLineLocation() {
    ListEmptyLines();

    int max_moves = emptylines_cnt;
    Loc* winning_moves = new Loc[max_moves];
    Loc* safe_moves = new Loc[max_moves];
    Loc* giver_moves = new Loc[max_moves];
    int winning_cnt = 0;
    int safe_cnt = 0;
    int giver_cnt = 0;

    //Classify
    for(int i = 0; i < emptylines_cnt; i++) {
        Loc move = emptylines[i];
        int r = move.row;
        int c = move.col;

        int r_dot1 = -1, c_dot1 = -1; //adjacent boxs
        int r_dot2 = -1, c_dot2 = -1;

        if (move.IsLineHorizontalLocation()) {
            r_dot1 = r;      //Box below the line
            c_dot1 = c - 1;
            r_dot2 = r - 2;  //Box above the line
            c_dot2 = c - 1;
        } else {
            r_dot1= r - 1;  //right
            c_dot1= c;
            r_dot2= r - 1;  //left
            c_dot2= c - 2;
        }

        //Check if this move completes a box (4th side = winning move)
        bool is_winning = false;
        if (r_dot1 >= 0 && c_dot1 >= 0) {
            if (CountBoxSides(r_dot1, c_dot1) == 3) {
                is_winning = true;
            }}
        if (r_dot2 >= 0&&c_dot2 >= 0) {
            if (CountBoxSides(r_dot2, c_dot2) == 3) {
                is_winning=true;
            }}

        //If winning mov
        if (is_winning) {
            winning_moves[winning_cnt++] = move;
            continue;
        }

        //Check if this move creates a 3-sided box
        bool is_giver = false;
        if (r_dot1 >= 0 && c_dot1 >= 0) {
            if (CountBoxSides(r_dot1, c_dot1) == 2) {
                is_giver = true;
            }
        }
        if (r_dot2 >= 0 && c_dot2 >= 0) { //gives the enemy a chance
            if (CountBoxSides(r_dot2, c_dot2) == 2) {
                is_giver = true;
            }
        }

        //two kinds of moves
        if (is_giver) {
            giver_moves[giver_cnt++] = move;
        } else {
            safe_moves[safe_cnt++] = move;
        }
    }

    //priority:
    //1.Winning moves
    //2.Safe moves
    //3.Giver moves LAST RESORT
    Loc selected;
    if (winning_cnt > 0) {
        //Choose random winning move
        selected = winning_moves[rand() % winning_cnt];
    } else if (safe_cnt > 0) {
        selected = safe_moves[rand() % safe_cnt];
    } else if (giver_cnt > 0) {
        selected = giver_moves[rand() % giver_cnt];
    } else {
        //should only occur if board is full else, you got a problem
        selected = emptylines[0];
        //cout<<"\nMake sure this appears after board else, cbeck selection \n\n" //tests
    }

    delete[] winning_moves;
    delete[] safe_moves;
    delete[] giver_moves;
    return selected;
}