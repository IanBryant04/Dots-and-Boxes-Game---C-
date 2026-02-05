//---------------------------------------------------------------------
// Name: Ian Bryant
// Email: iab5219@psu.edu
// Class: CMPSC 330
// Assignment: Homework 4
// Description:Board class for Dots and Boxes
//---------------------------------------------------------------------

#include <iostream>
#include "board.h"

using namespace std;

Board::Board(int dots_in_rows, int dots_in_cols) {
    dotsRows = dots_in_rows;
    dotsCols = dots_in_cols;
    
    //Calculate physical dimensions (2*n - 1)
    rows = 2 * dotsRows - 1;
    cols = 2 * dotsCols - 1;

    //Dynamic allocation of 2D array
    grid = new char*[rows];
    for (int i = 0; i < rows; i++) {
        grid[i] = new char[cols];
    }

    //Initialize board with dots
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i % 2 == 0 && j % 2 == 0)
                grid[i][j] = '.';
            else
                grid[i][j] = ' ';
        }
    }
}

//Destructor
Board::~Board() {
    for (int i = 0; i < rows; i++) {
        delete[] grid[i];
    }
    delete[] grid;
}

//Access operators
char& Board::operator()(int row, int col) {
    return grid[row][col];
}
char Board::operator()(int row, int col) const {
    return grid[row][col];
}

//Check if a specific box is complete
bool Board::IsBoxComplete(int row, int col) const {
    //Bounds check
    if (row < 0 || col < 0 || (row + 2) >= rows || (col + 2) >= cols) {
        return false;
    }

    //test for valid
    if (row % 2 != 0 || col % 2 != 0) {
        return false;
    }

    //Check all four sides (from first assignment)
    bool top    = (grid[row][col + 1] != ' ');
    bool bottom = (grid[row + 2][col + 1] != ' ');
    bool left   = (grid[row + 1][col] != ' ');
    bool right  = (grid[row + 1][col + 2] != ' ');

    return (top && bottom && left && right);
}

//Assign a box to a player if complete
//will retur  true if the box was just filled
bool Board::AssignBoxOwner(int row, int col, char player) {
    if (IsBoxComplete(row, col)) {
        // Check if the center is currently empty
        if (grid[row + 1][col + 1] == ' ') {
            //Convert to uppercase
            char owner = player;
            if (owner >= 'a' && owner <= 'z') {
                owner = owner - 32;
            }
            grid[row + 1][col + 1] = owner;
            return true;
        }
    }
    return false;
}

//Place a line and update score
int Board::PlaceLine(int row, int col, char player) {
    char lineChar = player;
    if (lineChar >= 'A' && lineChar <= 'Z') {
        lineChar = lineChar + 32;
    }
    
    grid[row][col] = lineChar;

    int boxesEarned = 0;

    // Check adjacent boxes based on line orientation
    if (row % 2 == 0) { //horizontal
        //Check box Above and Below
        if (AssignBoxOwner(row - 2, col - 1, player)) {
            boxesEarned++;
        }
        // Box Below
        if (AssignBoxOwner(row, col - 1, player)) {
            boxesEarned++;
        }
    } else { //vertical
        //Check box Left and Right
        if (AssignBoxOwner(row - 1, col - 2, player)) {
            boxesEarned++;
        }
        // Box Right
        if (AssignBoxOwner(row - 1, col, player)) {
            boxesEarned++;
        }
    }

    return boxesEarned;
}

bool Board::IsValidMove(int row, int col) const {
    //Check Bounds
    if (row < 0 || row >= rows || col < 0 || col >= cols)
        return false;

    //already occupied?
    if (grid[row][col] != ' ')
        return false;

    //Check if it is a Dot
    if (row % 2 == 0 && col % 2 == 0)
        return false;

    //Check if it is a Box Center
    if (row % 2 != 0 && col % 2 != 0)
        return false;

    return true;
}

//Fill arrays with all valid empty line locations
//Returns the count of empty lines
int Board::GetAllEmptyLineLocations(int* empty_rows, int* empty_cols) const {
    int count = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (IsValidMove(i, j)) {
                empty_rows[count] = i;
                empty_cols[count] = j;
                count++;
            }
        }
    }
    return count;
}

//Check if the board is full
bool Board::IsFull() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (IsValidMove(i, j)) {
                return false; // Found an empty spot
            }
        }
    }
    return true;
}

//Display the board (from other hw)
void Board::Print() const {
    //Print Column numbers (Tens digit)
    cout << "  ";
    for (int j = 0; j < cols; j++) {
        if (j >= 10 && j % 10 == 0) {
            cout << (j / 10);
        } else {
            cout << " ";
        }
    }
    cout << endl;

    //Print Column numbers (Ones digit)
    cout << "  ";
    for (int j = 0; j < cols; j++)
        cout << (j % 10);
    cout << endl;

    // Print Grid with Row numbers
    for (int i = 0; i < rows; i++) {
        // Print tens digit for row number
        if (i % 10 == 0) {
            cout << (i / 10);
        } else {
            cout << " ";
        }
        // Print ones digit for row number
        cout << (i % 10);

        // Print board row
        for (int j = 0; j < cols; j++) {
            cout << grid[i][j];
        }
        cout << endl;
    }
}