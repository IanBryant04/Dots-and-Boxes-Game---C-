

#ifndef BOARD_H
#define BOARD_H

class Board {
private:
    char** grid;
    int rows;      // Board rows (2*dots_rows - 1)
    int cols;      // Board cols (2*dots_cols - 1)
    int dotsRows;  // Original dots in rows
    int dotsCols;  // Original dots in cols

    // Helper functions
    bool IsBoxComplete(int row, int col) const;
    bool AssignBoxOwner(int row, int col, char player);

public:
    // Constructor and destructor
    Board(int dots_in_rows, int dots_in_cols);
    ~Board();

    char& operator()(int row, int col);
    char operator()(int row, int col) const;

    //Game functions
    int GetAllEmptyLineLocations(int* empty_rows, int* empty_cols) const;
    bool IsValidMove(int row, int col) const;
    bool IsFull() const;
    int PlaceLine(int row, int col, char player);
    
    void Print() const;
    
    int GetRows() const { return rows; }
    int GetCols() const { return cols; }
    int GetDotsRows() const { return dotsRows; }
    int GetDotsCols() const { return dotsCols; }
};

#endif