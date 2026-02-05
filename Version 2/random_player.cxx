//---------------------------------------------------------------------
// Name: Ian Bryant
// Email: iab5219@psu.edu
// Class: CMPSC 330
// Assignment: Homework 4
// Description: Implementation of the RandomPlayer class.
//---------------------------------------------------------------------
#include <cstdlib>
#include "random_player.h"

void RandomPlayer::SelectLineLocation(const Board& board, int& r, int& c) {
    //The maximum number of lines is less than the total grid cells.
    int max_possible_moves = board.GetRows() * board.GetCols();

    //Dynamically allocate memory
    int* empty_rows = new int[max_possible_moves];
    int* empty_cols = new int[max_possible_moves];

    //arrays with all valid moves
    int count = board.GetAllEmptyLineLocations(empty_rows, empty_cols);


    if (count > 0) {
        int index = rand() % count; //gets random point
        r = empty_rows[index];
        c = empty_cols[index];
    } else {
        //if occurs we got a problem
        r = -1;
        c = -1;
    }

    delete[] empty_rows;
    delete[] empty_cols;
}