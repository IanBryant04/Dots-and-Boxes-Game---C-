//---------------------------------------------------------------------
// Name: Ian Bryant
// Email: iab5219@psu.edu
// Class: CMPSC 330
// Program 2
// Due Date: October 31st, 2025
// Description: This program reads a file from the command line with a series of moves
// and determines the winner
// Acknowledgement:
// 1. Structs have not been taught in 330 but I am in CMPSC 313 and need to use them in C
// extensively so I used that here with the PlayerData struct
//2. Had to google how to dynamically allocate 2d char array
//https://stackoverflow.com/questions/2614249/dynamic-memory-for-2d-char-array
//3. Found recursive search and sort for the players
//https://www.geeksforgeeks.org/dsa/recursive-selection-sort/#
//---------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct PlayerData { //taught in 313
    char name;
    int boxCount;
};

int findPlayerIndex(PlayerData* players, int playerCount, char playerName, int index=0);
void sortPlayers(PlayerData* players, int count, int start=0);
bool isBoxComplete(char** board, int row, int col, int maxRows, int maxCols);
bool assignBoxOwner(char** board, int row, int col, char player, int maxRows, int maxCols);
void checkAndAssignBoxes(char** board, int x, int y, char player, int maxRows, int maxCols, PlayerData* players, int playerCount);

int main(int argc, char* argv[]) {
    //Hard-coded file path
    string filename; //used in testing

    //Uses command line argument if provided else uses my hard coded file path
    if (argc>1) {
        filename=argv[1];
    }
    //Errors with opening file
    ifstream infile(filename.c_str());
    if (!infile) {
        cout<<"Error could not open file"<<endl;
        return 1;
    }

    int rows, cols;
    if (!(infile>>rows>>cols)) {
        cout<<"error! failed to read board dimensions"<<endl;
        return 1;
    }

    //Dynamic allocate the dimensions
    int boardRows=2*rows-1;
    int boardCols=2*cols-1;
    char** board=new char*[boardRows];
    for (int i=0; i<boardRows; i++) {
        board[i]=new char[boardCols];
    }

    //Create board with dots
    for (int i=0; i<boardRows; i++) {
        for (int j=0; j<boardCols; j++) {
            if (i%2==0 && j%2==0)
                board[i][j]='.';   //dots
            else
                board[i][j]=' ';  //empty spaces
        }
    }

    //Dynamic allocation for players
    PlayerData* players=new PlayerData[100]; // Max 100 players USES STRUCT
    int playerCount=0;
    string player;
    int x, y;
    bool gameValid=true;
    int errorX=-1, errorY=-1;

    while (infile>>player) {
        if (player=="END")
            break;

        infile>>x>>y;
        char playerChar=player[0];

        //Convert to lowercase for lines
        if (playerChar>='A' && playerChar<='Z') {
            playerChar=playerChar+32;  //Convert to lowercase
        }

        //Add player if not exists
        if (findPlayerIndex(players, playerCount, playerChar)==-1) {
            players[playerCount].name=playerChar;
            players[playerCount].boxCount=0;
            playerCount++;
        }

        //Check for invalid move
        if (x<0 || y<0 || x>=boardRows || y>=boardCols) {
            gameValid=false;
            errorX=x;
            errorY=y;
            break;
        }
        //Check if placing on dot
        if (x%2==0 && y%2==0) {
            gameValid=false;
            errorX=x;
            errorY=y;
            break;
        }
        //Check if line already exists
        if (board[x][y]!=' ') {
            gameValid=false;
            errorX=x;
            errorY=y;
            break;
        }

        board[x][y]=playerChar;

        //Check and assign boxes
        checkAndAssignBoxes(board, x, y, playerChar, boardRows, boardCols, players, playerCount);
    }

    infile.close();

    //Print final board with row/column numbers --------------------------------------
    //Column numbers tens row
    cout<<"  ";
    for (int j=0; j<boardCols; j++) {
        if (j>=10 && j%10==0) {
            cout<<(j/10);
        } else {
            cout<<" ";
        }
    }
    cout<<endl;

    //Column numbers ones row
    cout<<"  ";
    for (int j=0; j<boardCols; j++)
        cout<<(j%10);
    cout<<endl;

    //Print board with row numbers
    for (int i=0; i<boardRows; i++) {
        // Print tens digit for row number (only when divisible by 10)
        if (i%10==0) {
            cout<<(i/10);
        } else {
            cout<<" ";
        }
        // Print ones digit for row number
        cout<<(i%10);

        // Print board row
        for (int j=0; j<boardCols; j++) {
            if (!gameValid && i==errorX && j==errorY) {
                cout<<'X';
            } else {
                cout<<board[i][j];
            }
        }
        cout<<endl;
    }

    //Sort players lexicographically
    sortPlayers(players, playerCount);

    // Find max boxes
    int maxBoxes=0;
    for (int i=0; i<playerCount; i++) {
        if (players[i].boxCount>maxBoxes) {
            maxBoxes=players[i].boxCount;
        }
    }

    //Count up the winners
    int winnerCount=0;
    if (gameValid) {
        for (int i=0; i<playerCount; i++) {
            if (players[i].boxCount==maxBoxes) {
                winnerCount++;
            }
        }
    } else {
        //Find the cheating player
        for (int i=0; i<playerCount; i++) {
            if (players[i].name!=player[0]) {
                if (players[i].boxCount==maxBoxes) {
                    winnerCount++;
                }
            }
        }
        //If cheater = max boxes, redo
        int cheaterIdx=findPlayerIndex(players, playerCount, player[0]);
        if (cheaterIdx!=-1 && players[cheaterIdx].boxCount==maxBoxes) {
            maxBoxes=0;
            winnerCount=0;
            for (int i=0; i<playerCount; i++) {
                if (i!=cheaterIdx && players[i].boxCount>maxBoxes) {
                    maxBoxes=players[i].boxCount;
                }
            }
            for (int i=0; i<playerCount; i++) {
                if (i!=cheaterIdx && players[i].boxCount==maxBoxes) {
                    winnerCount++;
                }
            }
        }
    }

    //Print results
    for (int i=0; i<playerCount; i++) {
        cout<<"Player "<<players[i].name<<" has "<<players[i].boxCount<<" box";
        if (players[i].boxCount!=1) cout<<"es";

        if (!gameValid && players[i].name==player[0]) {
            // This player cheated and loses
            cout<<" (cheater)";
        } else if (winnerCount>1 && players[i].boxCount==maxBoxes) {
            cout<<" (tie)";
        } else if (winnerCount==1 && players[i].boxCount==maxBoxes) {
            cout<<" (win)";
        }
        cout<<"."<<endl;
    }

    //Deallocate memory
    for (int i=0; i<boardRows; i++) {
        delete[] board[i];
    }
    delete[] board;
    delete[] players;

    return 0;
}

//Function to find or add a player (using recursion)
int findPlayerIndex(PlayerData* players, int playerCount, char playerName, int index) {
    if (index>=playerCount) {
        return -1; //Not found
    }
    if (players[index].name==playerName) {
        return index;
    }
    return findPlayerIndex(players, playerCount, playerName, index+1);
}

//RECURSIVE FUNCTION to sort players lexicographically
void sortPlayers(PlayerData* players, int count, int start) {
    if (start>=count-1) {
        return;
    }
    //Find minimum in remaining array
    int minIndex=start;
    for (int i=start+1; i<count; i++) {
        if (players[i].name<players[minIndex].name) {
            minIndex=i;
        }
    }

    //Swap if needed
    if (minIndex!=start) {
        PlayerData temp=players[start];
        players[start]=players[minIndex];
        players[minIndex]=temp;
    }

    //Recursive call for rest of array
    sortPlayers(players, count, start+1);
}

//Check if a box is completed at position (row, col) - top-left corner of box
bool isBoxComplete(char** board, int row, int col, int maxRows, int maxCols) {
    // Ensure the given position is a valid top-left corner of a box
    if (row<0 || col<0 || (row+2)>=maxRows || (col+2)>=maxCols) {
        return false;
    }

    // Boxes only exist at even row/column coordinates
    if (row%2!=0 || col%2!=0) {
        return false;
    }

    bool top=false;
    bool bottom=false;
    bool left=false;
    bool right=false;

    //Check each side
    if(board[row][col+1]!=' ') {
        top=true;
    }

    if(board[row+2][col+1]!=' ') {
        bottom=true;
    }

    if(board[row+1][col]!=' ') {
        left=true;
    }

    if(board[row+1][col+2]!=' ') {
        right=true;
    }

    //A box is complete only if all four sides exist
    if(top && bottom && left && right) {
        return true;
    } else {
        return false;
    }
}


//Assign box owner and return true
bool assignBoxOwner(char** board, int row, int col, char player, int maxRows, int maxCols) {
    if (isBoxComplete(board, row, col, maxRows, maxCols)) {
        if (board[row+1][col+1]==' ') {
            // Convert to uppercase for box owner
            if (player>='a' && player<='z') {
                board[row+1][col+1]=player-32;
            } else {
                board[row+1][col+1]=player;
            }
            return true;
        }
    }
    return false;
}

//Checks boxes around a new line
void checkAndAssignBoxes(char** board, int x, int y, char player, int maxRows, int maxCols, PlayerData* players, int playerCount) {
    int playerIdx=findPlayerIndex(players, playerCount, player);
    //Check boxes that could be affected by this line
    if (x%2==0) {
        // Horizontal line - check box above and below
        if (assignBoxOwner(board, x-2, y-1, player, maxRows, maxCols)) {
            if (playerIdx!=-1) players[playerIdx].boxCount++;
        }
        if (assignBoxOwner(board, x, y-1, player, maxRows, maxCols)) {
            if (playerIdx!=-1) players[playerIdx].boxCount++;
        }
    } else {
        // Vertical line - check box left and right
        if (assignBoxOwner(board, x-1, y-2, player, maxRows, maxCols)) {
            if (playerIdx!=-1) players[playerIdx].boxCount++;
        }
        if (assignBoxOwner(board, x-1, y, player, maxRows, maxCols)) {
            if (playerIdx!=-1) players[playerIdx].boxCount++;
        }
    }
}