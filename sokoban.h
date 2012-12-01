#include "node_b.h"
#include "simulate.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>
#include <set>

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

//Operator used to order elements in a priority queue
bool operator<(const node_b &a, const node_b &b);

//Function particular to a thread implementation. This instructs each working
//thread to take elements from the queue and push in sons until a solution is
//found
void analyze();

//Function particular to omp implementation. It corresponds to a task that has
//to be analyzed by a particular node
void analyze(node_b par);
