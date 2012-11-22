#ifndef BOARD_H
#define BOARD_H
#include "board.h"
#endif

#include <string>
#include <iostream>

using namespace std;

//Performs the set of movements defined on s on the board b, assuming that they
//are defined from the player's position on b. It outputs the results as a 
//succession of prints, gicing the sensation of an animation if the screen is 
//big enough.
//It is assumed that the set of moves given is valid. If not, the behaviour is
//undefined
void run_simulation(board b, string s);
