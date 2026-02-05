#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include "common.h"
#include "board.h"
#include "player.h"
using namespace std;

#include <unistd.h>  // usleep()
#include <dlfcn.h>

string GameMaster(const vector<string>& libpaths, vector<int>& scores, vector<double>& timespents, double sleep_sec);

void CloseLibs();

int main(int argc, char* argv[])
{
    // argv[0] : program name
    // argv[1] : 1st player library path
    // argv[2] : 2nd player library path
    // argv[3] : 3rd player library path
    if(argc <= 2)
        return 0;

    vector<string> libpaths;
    vector<int>    scores;
    vector<double> timespents;
    for(int i=1; i<argc; i++)
    {
        libpaths.push_back(argv[i]);
        scores.push_back(0);
        timespents.push_back(0);
    }

    double sleep_sec = 0.00;
    string result = GameMaster(libpaths, scores, timespents, sleep_sec);
    CloseLibs();
}

void ClearConsole()
{
    system("clear");
}

void SleepInSec(double sleep_sec)
{
    int sleep_microsecond = sleep_sec * 1000000;
    usleep(sleep_microsecond);
}

vector<void*> lst_player_libhandle;
typedef IPlayer* (*CreatePlayerType)();
string LoadPlayer
    ( IPlayer*& iplayer
    , string libpath
    , int  board_rows   // the size of board (including dots, lines, and boxes)
    , int  board_cols   // the size of board (including dots, lines, and boxes)
    , char box_type     // the character for the player's boxes
    , char line_type    // the character for the player's lines
    )
{
    void* player_libhandle = dlopen(libpath.c_str(), RTLD_LAZY);
    if(player_libhandle == nullptr)
    {
        return "cannot load " + libpath;
    }
    lst_player_libhandle.push_back(player_libhandle);
    void* pfunc = dlsym(player_libhandle, "PlayerFactory");
    if(pfunc == nullptr)
    {
        return "cannot find PlayerFactory() function defined as extern C";
    }
    CreatePlayerType CreatePlayer = (CreatePlayerType)pfunc;
    iplayer = CreatePlayer();
    iplayer->Init(board_rows, board_cols, box_type, line_type);
    if(iplayer == nullptr)
    {
        return "cannot create player using PlayerFactory() function";
    }
    return "";
}
void CloseLibs()
{
    for(void* player_libhandle : lst_player_libhandle)
        dlclose(player_libhandle);
    lst_player_libhandle.clear();
}

