#include "node_b.h"
#include "simulate.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <unordered_map>

using namespace std;

//Calls the print method on the object given, adding a tab.
void call_print(position p);

//Main method to be executed
int main(int argc, char **argv);

//Auxiliary method used for finding a solution. Receives the initial board
//for which a solution is to be found. Returns a string containing the path
//that must be followed to push each box into a goal, or the string 'E' if such
//path cannot be found
string push_to_goals(board in);

