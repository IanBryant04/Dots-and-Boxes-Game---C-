#include <iostream>
#include <time.h>
#include <chrono>
#include <assert.h>
#include <vector>
#include "common.h"
#include "board.h"
#include "player.h"


void ClearConsole();
void SleepInSec(double sleep_sec);
extern void* a_player_libhandle;
extern void* b_player_libhandle;
string LoadPlayer(IPlayer*& ab_player, string libpath, int board_rows, int board_cols, char box_type, char line_type);
void CloseLibs();

struct PlayerInfo
{
public:
    IPlayer* iplayer;
    int      score;
    char     box;
    char     line;
    double   timespent;
    PlayerInfo* next; // play-list (linked list)
    PlayerInfo* prev; // play-list (linked list)
    PlayerInfo(IPlayer* p, char b, char l)
    {
        iplayer = p;
        score = 0;
        box = b;
        line = l;
        timespent = 0;
    }
};

void PrintBoard(int iter, const Board& board, const vector<PlayerInfo>& players)
{
    // print board
    cout << board;
    cout << endl;

    // print iteration number
    cout << "iter : " << iter << endl;

    // find winning scores and winner numbers
    int maxscore     = 0;
    int maxscore_cnt = 0;
    for(int i=0; i<players.size(); i++)
    {
        if(maxscore < players[i].score)
        {
            maxscore = players[i].score;
            maxscore_cnt = 0;
        }
        if(maxscore == players[i].score)
            maxscore_cnt ++;
    }

    // print player status (boxes, win/tie)
    for(int i=0; i<players.size(); i++)
    {
        cout << "Player " << players[i].box << " (" << players[i].iplayer->PlayerInfo() << ") has ";
        if(players[i].score >= 2) cout << players[i].score << " boxes";
        else                      cout << players[i].score << " box";
        cout << " and spent " << players[i].timespent*100 << " msec";
        if(maxscore == players[i].score)
        {
            if(maxscore_cnt == 1) cout << " (win)." << endl;
            else                  cout << " (tie)." << endl;
        }
        else
        {
            cout << "." << endl;
        }
    }
    cout << endl;
}

// send add-line event to players
void EventAddLine(const vector<PlayerInfo>& players, char line, const Loc& loc)
{
    for(int j = 0; j < players.size(); j++)
    {
        if(players[j].next == nullptr)
            continue;
        players[j].iplayer->EventAddLine(line, loc);
    }
}

// send add-box event to players
void EventAddBox(const vector<PlayerInfo>& players, char box, const Loc& loc)
{
    for(int j = 0; j < players.size(); j++)
    {
        if(players[j].next == nullptr)
            continue;
        players[j].iplayer->EventAddBox(box, loc);
    }
}

// update box with player box info, and return true (for gaining box) or false (for not gaining box)
bool AddBox(Board& board, int row, int col, char box, const vector<PlayerInfo>& players)
{
    assert(Loc(row,col).IsBoxLocation());
    if(board(row,col) == ' '
    && board(row+1,col) != ' '
    && board(row-1,col) != ' '
    && board(row,col+1) != ' '
    && board(row,col-1) != ' ')
    {
        board(row,col) = box;
        EventAddBox(players, box, Loc(row,col));
        return true;
    }
    return false;
}

string GameMaster(const vector<string>& libpaths, vector<int>& scores, vector<double>& timespents, double sleep_sec)
{
    ///////////////////////////////////////////////////
    // create board
    int board_rows, board_cols, board_blanklinecount;
    cin >> board_rows >> board_cols;
    cout << endl;
    Board board;
    board.AllocateBoard(board_rows, board_cols, board_blanklinecount);

    ///////////////////////////////////////////////////
    // load players
    vector<PlayerInfo> players;
    int ibox = 0;
    for(int i=0; i<libpaths.size(); i++)
    {
        string libpath  = libpaths[i];
        IPlayer* player = nullptr;
        char box_type  = 'A' + ibox;
        char line_type = 'a' + ibox;
        ibox++;
        if(box_type == 'E' || box_type == 'X')
        {
            box_type  = 'A' + ibox;
            line_type = 'a' + ibox;
            ibox++;
        }

        string message = "";
        try
        {
            message = LoadPlayer(player, libpath, board_rows, board_cols, box_type, line_type);
            if(message != "")
            {
                cout << "Library error while loading a player (" << libpath << ")." << endl;
                if(player != nullptr)
                    player->Close();
                continue;
            }
            players.push_back(PlayerInfo(player, box_type, line_type));
        }
        catch(...)
        {
            cout << "Exception while loading a player (" << libpath << ")." << endl;
            if(player != nullptr)
                player->Close();
            continue;
        }
    }

    ///////////////////////////////////////////////////
    // set player sequence and first player
    int iter = 0;
    for(int i=0; i<players.size(); i++)
    {
        int next_idx = (i+1) % players.size();
        int prev_idx = (i-1) % players.size();
        players[i].next = &(players[next_idx]);
        players[i].prev = &(players[prev_idx]);
    }
    PlayerInfo* player = &(players[0]);

    ///////////////////////////////////////////////////
    // play game
    while(true)
    {
        iter++;

        ////////////////////////////////////////////////////////////
        // stop if there is no player to play
        if(player == nullptr)
            break;
        // stop if the board is full
        if(board_blanklinecount == 0)
            break;

        // print play-log
        if(sleep_sec == 0)
            cout << "Player " << player->box;

        ////////////////////////////////////////////////////////////
        // determine player's line location
        // if the player put a line into a invalid location, then makr the player lose
        Loc loc;
        double timespent = 0;
        try
        {
            auto begin = chrono::high_resolution_clock::now();
            loc = player->iplayer->SelectLineLocation();
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
            player->timespent += (elapsed.count() * 1e-9);
            if(loc.IsBoxLocation()) throw "lose";
            if(loc.IsDotLocation()) throw "lose";
            if(board(loc) != ' '  ) throw "lose";
        }
        catch(...)
        {
            // if the player put line into a wrong location or throw exception,
            // make the player have -1 score (invalid move) and
            // remove the player from play-list (linked-list)
            player->score = -1;
            if(player->next == player)
            {
                // if player is the last player, make the linked-list empty
                player = nullptr;
            }
            else
            {
                // if the player is not the last player, remove the player in the linked-list, and select the next player
                PlayerInfo* next_player = player->next;
                player->next->prev = player->prev;
                player->prev->next = player->next;
                player->next = nullptr;
                player->prev = nullptr;
                player = next_player;
            }
            // print play-log
            if(sleep_sec == 0)
                cout << " made an invalid move." << endl;
            continue;
        }

        ////////////////////////////////////////////////////////////
        // add the line into the board
        assert(loc.IsLineLocation());
        assert(board(loc) == ' '   );
        board(loc) = player->line;
        board_blanklinecount --;
        EventAddLine(players, player->line, loc);

        ////////////////////////////////////////////////////////////
        // if the player gain a box, then
        // mark the box owner and
        // make gainbox true
        int gainbox = 0;
        if(loc.IsLineHorizontalLocation())
        {
            if(loc.row-1 >= 0              ) if(AddBox(board, loc.row-1, loc.col, player->box, players)) { player->score++; gainbox++; }
            if(loc.row+1 <  board.GetRows()) if(AddBox(board, loc.row+1, loc.col, player->box, players)) { player->score++; gainbox++; }
        }
        if(loc.IsLineVerticalLocation())
        {
            if(loc.col-1 >= 0              ) if(AddBox(board, loc.row, loc.col-1, player->box, players)) { player->score++; gainbox++; }
            if(loc.col+1 <  board.GetCols()) if(AddBox(board, loc.row, loc.col+1, player->box, players)) { player->score++; gainbox++; }
        }

        ////////////////////////////////////////////////////////////
        // print board or play-log
        if(sleep_sec == 0)
        {
            cout << " add a line at (" << loc.row << "," << loc.col << ")";
            if(gainbox == 1)
                cout << " and gain " << gainbox << " box." << endl;
            else if(gainbox >= 2)
                cout << " and gain " << gainbox << " boxes." << endl;
            else
                cout << "." << endl;
        }
        else
        {
            ClearConsole();
            PrintBoard(iter, board, players);
            SleepInSec(sleep_sec);
        }

        ////////////////////////////////////////////////////////////
        // if the player does not gain a box
        // move to the next player
        if(gainbox == 0)
            player = player->next;
    }

    ////////////////////////////////////////////////////////////
    // print board
    if(sleep_sec != 0) ClearConsole();
    else               cout << endl;
    PrintBoard(iter, board, players);

    for(int i = 0; i < players.size(); i++)
    {
        scores    [i] = players[i].score;
        timespents[i] = players[i].timespent;
    }
    return "";
}
