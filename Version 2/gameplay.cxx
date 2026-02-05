//---------------------------------------------------------------------
// Name: Ian Bryant
// Email: iab5219@psu.edu
// Class: CMPSC 330
// Program 4
// Due Date: November 21, 2025
// Description: Main gameplay loop for Dots and Boxes
//Acknowledgments:
//1.I worked alone on this assignment
//2.This is where I found the 'optimal' strategy for the game https://cdn.aaai.org/ojs/8144/8144-13-11671-1-2-20201228.pdf
//3. I copied alot from my first homework so here is those acknowledgements
//   3a. Had to google how to dynamically allocate 2d char array
//https://stackoverflow.com/questions/2614249/dynamic-memory-for-2d-char-array
//   3b. Found recursive search and sort for the players
//https://www.geeksforgeeks.org/dsa/recursive-selection-sort/#
//4. Looked here for all my info on using acess operators https://en.cppreference.com/w/cpp/language/operator_member_access.html
//---------------------------------------------------------------------
#include <iostream>
#include <string>
#include "board.h"
#include "random_player.h"
#include "strategic_player.h"

using namespace std;

int main() {
    //Read Game Settings
    int rows, cols;
    if (!(cin >> rows >> cols)) //case no input
        return 0;

    string user1, type1;
    cin >> user1 >> type1;

    string user2, type2;
    cin>> user2 >> type2;

    //Create Board and Players
    //uses dynamic memory
    Board* board =new Board(rows, cols);
    
    RandomPlayer randomBot;
    StrategicPlayer strategicBot;

    //Game State Variables
    int turn = 1; //player 1
    int score1 = 0;
    int score2 = 0;
    

    //Game Loop
    while (!board->IsFull()) {
        int r, c;
        string currentName;
        string currentType;

        //Identify current player
        if (turn == 1) {
            currentName = user1;
            currentType = type1;
        } else {
            currentName = user2;
            currentType = type2;
        }

        if (currentType == "Random") {
            randomBot.SelectLineLocation(*board, r, c);
        } else {
            strategicBot.SelectLineLocation(*board, r, c);
        }

        //Print Log to Console for actions
        cout << currentName << " " << r << " " << c << endl;

        //Apply Move and Check Score
        //We pass the first char of the player name
        int boxesEarned = board->PlaceLine(r, c, currentName[0]);

        //Update Scores
        if (turn == 1) {
            score1 += boxesEarned;
        } else {
            score2 += boxesEarned;
        }

        //If boxesEarned > 0, the current player keeps their turn.
        if (boxesEarned == 0) {
            turn = (turn == 1) ? 2 : 1;
        }
    }

    cout << "END" << endl;
    
    //Print finished board
    board->Print();

    //Determine the winner and print
    string status1, status2;
    if (score1 > score2) {
        status1 = "(win)";
        status2 = "";
    } else if (score2 > score1) {
        status1 = "";
        status2 = "(win)";
    } else {
        status1 = "(tie)";
        status2 = "(tie)";
    }

    cout << "Player " << user1 << " has " << score1 << " boxes";
    if (!status1.empty())
        cout << " " << status1;
    cout << "." << endl;

    cout << "Player " << user2 << " has " << score2 << " boxes";
    if (!status2.empty())
        cout << " " << status2;
    cout << "." << endl;

    delete board;

    return 0;
}