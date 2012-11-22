#include "node_p.h"

#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include <cstring>

using namespace std;

class board
{
    public:
        int num_rows;
        int num_cols;
        char **elements;
        position player;
        vector<position> boxes;
        vector<position> empty_goals;

        //Method that builds a board from a vector containing the lines with a string
        //representation of such board
        board(vector<string> lines);

        //Constructs a copy of the board
        board(const board &b);

        void print();

        //Moves the player to the destination indicated by p and performs the apropriate
        //updates on the object.
        //Returns true if the update was successfull, false otherwise
        bool move_player(position p);

        //Moves a box to the specified destination and does the apropriate update on the
        //parameter that was passed and on the object.
        //Returns true if the operation was successful, false otherwise
        bool push_box(position &box, position dest);

        //Finds the shortest path to get from a to be in this board and returns
        //it in the form of a string. Returns 'E' if there is no valid path.
        //This is done using BFS.
        string find_path(position a, position b);
};
