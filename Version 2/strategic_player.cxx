//---------------------------------------------------------------------
// Name: Ian Bryant
// Email: iab5219@psu.edu
// Class: CMPSC 330
// Assignment: Homework 4
// Description: Implementation of the StrategicPlayer class.
//---------------------------------------------------------------------
#include <cstdlib>
#include "strategic_player.h"

//check for lines
int StrategicPlayer::CountBoxSides(const Board& board, int r_dot, int c_dot) const {
    //r_dot and c_dot are corners
    //is this a valid box position?
    if (r_dot < 0 || c_dot < 0 || (r_dot + 2) >= board.GetRows() || (c_dot + 2) >= board.GetCols()) {
        return 5; //flags invalid box
    }
    if (r_dot % 2 != 0 || c_dot % 2 != 0) {
        return 5; //Not a dot coordinate
    }

    int count = 0;
    //Top line (r_dot, c_dot + 1)
    if (board(r_dot, c_dot + 1) != ' ')
        count++;
    //Bottom line
    if (board(r_dot + 2, c_dot + 1) != ' ')
        count++;
    //Left line
    if (board(r_dot + 1, c_dot) != ' ')
        count++;
    //Right
    if (board(r_dot + 1, c_dot + 2) != ' ')
        count++;

    return count;
}


void StrategicPlayer::SelectLineLocation(const Board& board, int& r, int& c) {
    int max_possible_moves=board.GetRows()*board.GetCols();

    //Dynamically allocated arrays 
    int* all_moves_r = new int[max_possible_moves];
    int* all_moves_c = new int[max_possible_moves];

    int* winning_r = new int[max_possible_moves];
    int* winning_c = new int[max_possible_moves];
    int winning_count=0;

    int* safe_r = new int[max_possible_moves];
    int* safe_c = new int[max_possible_moves];
    int safe_count=0;

    int* giver_r=new int[max_possible_moves];
    int* giver_c=new int[max_possible_moves];
    int giver_count = 0;

    //all empty line locations
    int total_moves = board.GetAllEmptyLineLocations(all_moves_r, all_moves_c);

    //classify moves
    for (int i = 0; i < total_moves; i++) {
        int r_move = all_moves_r[i];
        int c_move = all_moves_c[i];

        //FIX 1: Calculate adjacent box coordinates (r_dot1, c_dot1, r_dot2, c_dot2)
        int r_dot1 = -1, c_dot1 = -1; // Box 1 top-left dot coordinate
        int r_dot2 = -1, c_dot2 = -1; // Box 2 top-left dot coordinate (if exists)

        if (r_move % 2 == 0) { //Horizontal line
            r_dot1 = r_move;      //Box below
            c_dot1 = c_move - 1;
            r_dot2 = r_move - 2;  //Box above
            c_dot2 = c_move - 1;
        } else { //Vertical line
            r_dot1 = r_move - 1;  //Box right
            c_dot1 = c_move;
            r_dot2 = r_move - 1;  //Box left
            c_dot2 = c_move - 2;
        }

        //Check Box 1 for Win
        if (r_dot1 != -1 && CountBoxSides(board, r_dot1, c_dot1) == 3) {
            // Found a win. Record it and move to the next available line.
            winning_r[winning_count] = r_move;
            winning_c[winning_count] = c_move;
            winning_count++;
            continue; // Immediately jumps to the start of the next iteration (i++)
        }

        //Check Box 2 for Win
        if (r_dot2 != -1 && CountBoxSides(board, r_dot2, c_dot2) == 3) {
            // Found a win. Record it and move to the next available line.
            winning_r[winning_count] = r_move;
            winning_c[winning_count] = c_move;
            winning_count++;
            continue; // Immediately jumps to the start of the next iteration (i++)
        }

        //Only moves that did NOT trigger should make it here

        bool is_giver = false;

        // Check Box 1 sets up opponent for a win (current side count is 2)
        if (r_dot1!= -1 && CountBoxSides(board, r_dot1, c_dot1) == 2) {
            is_giver = true;
        }
        //Check Box 2 for Giver move
        if (r_dot2!= -1 && CountBoxSides(board, r_dot2, c_dot2) == 2) {
            is_giver = true;
        }

        //Categorize based on outcome
        if (is_giver) {
            //Add to Giver list
            giver_r[giver_count] = r_move;
            giver_c[giver_count] = c_move;
            giver_count++; // Increment the counter
        } else {
            //Add to Safe list
            safe_r[safe_count] = r_move;
            safe_c[safe_count] = c_move;
            safe_count++;
        }
    } // END of the FOR loop (Extra brace removed)

    //Selection Priority (Must be guaranteed to beat Random)
    int index;
    if (winning_count > 0) {
        index = rand() % winning_count;
        r = winning_r[index];
        c = winning_c[index];
    } else if (safe_count > 0) {
        index = rand() % safe_count;
        r = safe_r[index];
        c = safe_c[index];
    } else if (giver_count > 0) {
        //sacrifice/giver moves (last hope)
        index = rand() % giver_count;
        r = giver_r[index];
        c = giver_c[index];
    } else {
        //This case should not occur unless board is full
        r = -1; c = -1;
    }

    //delete all
    delete[] all_moves_r;
    delete[] all_moves_c;
    delete[] winning_r;
    delete[] winning_c;
    delete[] safe_r;
    delete[] safe_c;
    delete[] giver_r;
    delete[] giver_c;
}