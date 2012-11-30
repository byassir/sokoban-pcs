#include "node_p.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <queue>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <algorithm>

using namespace std;

//Function used to sort the boxes of the vector
bool box_cmp(position a, position b);

class board
{
    public:
        int num_rows;
        int num_cols;
        int **distance;
        char **elements;
        position player;
        vector<position> boxes;
        vector<position> empty_goals;
        bool **s_deadlock;

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

        //Defines wether a position will lead to a dead configuration
        bool is_deadlock(int x, int y);

        //Returns the special hash key for this board
        string get_key();

    private:
        //Finds all positions that are deadlocked due to wall configuration
        void find_deadlocks();
        
        //Determines wether a position is in deadlock due to the box configuration
        bool dynamic_deadlock(int x, int y, bool **checked);

        //Updates the possible distance from each position to the nearest free
        //goal
        void update_distances();

        //Function that determines the reachable area for the player in the 
        //given board
        position find_area();
};
