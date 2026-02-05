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

./dotsboxes ./IanBryantiab5219.so ./random_player.so
./dotsboxes ./IanBryantiab5219.so ./strategic_player.so
./dotsboxes ./IanBryantiab5219.so ./strategic_player.so ./random_player.so
./dotsboxes ./strategic_player.so ./random_player.so ./strategic_player.so 
from instructions with my file name

To run against random player: ->these are the main two lines youll need everything is compiled already
echo "4 4" | ./dotsboxes ./IanBryantiab5219.so ./random_player.so

To run against strategic player: ->these are the main two lines youll need everything is compiled already
echo "4 4" | ./dotsboxes ./IanBryantiab5219.so ./strategic_player.so
      ^^board dimensions obviously

To compile my player engine library: #files should already exist, so 
g++ -shared -fPIC -ansi -pedantic -std=c++14 IanBryantiab5219.cxx board.cxx -o IanBryantiab5219.so

To compile the game master:
g++ -ansi -pedantic -std=c++14 board.cxx dotsboxesgm.cxx main.cxx -ldl -o dotsboxes



HOW PROGRAMS WERE RESTRUCTURED

The largest difference between my current logic and the previous homework is the use of "safe" moves.
In the previous implementation the program
	1. searches for "winning move"
	2. if no winning block then perform random valid move
For homework 5, I now consider for if my move is creating a 3-sided box.
	1.Searches for winning move
	2.If no winning block check if it will make block 3-sided
	3.if it does go to different position, therefore not allowing the strategic player the ability to close it
Then of course compared to the first program where there was no logic, and the moves were simply
read from the file, so really the only relation is reading moves/outputing them.



WHAT I LEARNED:

The biggest thing I learned as I did each homework was how to separate interface from implementation. In HW4 my player got the board directly as a parameter,
but in HW5 I had to maintain my own copy and update it whenever anything happened in the game.
I also learned about using header files properly and how inheritance works in C++.
The EventAddLine() and EventAddBox() were confusing since I had to track what other players moves not just mine, but eventually I was able to gain an understanding.
The extern command, shared library stuff and bash commands were new to me. Considering none of that was covered at any point in the course that was easily the
most difficult part of the project, and the majority of my time was spent doing that.