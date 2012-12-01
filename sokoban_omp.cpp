#include "sokoban.h"
#include <thread>

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        cout << "Usage: OMP_NUM_THREADS=num_threads ./sokoban_seq input_file" << endl;
        return 0;
    }

    ifstream in_file;
    string in_line;
    char dummy;
    vector<string> input;
    int num_rows;
    

    in_file.open(argv[1]);
    //Read the number of rows and the linefeed character
    in_file >> num_rows;
    getline(in_file, in_line);

    //Read the rest of the board
    for(int i = 0; i < num_rows; ++ i)
    {
        getline(in_file, in_line);
        input.push_back(in_line);
    }

    board b(input);

//    b.print();

    string sol = push_to_goals(b);
    cout << sol << endl;
//    run_simulation(b, sol);
    return 1;
}

priority_queue<node_b> q;
set<string> visited;
string solution;

string push_to_goals(board in)
{
    node_b next_node(in, "");
    q.push(next_node);
    solution = "E";

    #pragma omp parallel
    {
        #pragma omp single
        {
            while(solution.compare("E") == 0) 
            {
                //If no solution has been found, then spawn a new task
                if(!q.empty())
                {
                    #pragma omp critical
                    {
                        next_node = q.top();
                        q.pop();
                    }

                    #pragma omp task firstprivate(next_node)
                        analyze(next_node);
                }
            }
        }
    }

    return solution;
}

void analyze(node_b par)
{
    //Iterate while there is no solution found
    while(solution.compare("E") == 0)
    {
        vector<node_b> to_push;
        board par_b = par.current;

        //Check if this is a final solution
        if(par_b.empty_goals.size() == 0)
        {
            #pragma omp critical
                solution = par.path;
            return;
        }

        //Check if this element has already been visited
        bool leave = false;
        #pragma omp critical
            if(!visited.insert(par_b.get_key()).second)
                leave = true;

        if(leave) return;

        //For each box that is present on the board, try pushing it in every
        //valid direction
        for(int i = 0; i < par_b.boxes.size(); ++ i)
        {
            position box(par_b.boxes[i]);

            //Add all valid sons

            //UP
            //Check that the box can be moved there
            if((par_b.elements[box.x - 1][box.y] != '#') &&
               (par_b.elements[box.x - 1][box.y] != '*') &&
               (par_b.elements[box.x - 1][box.y] != '$') )
            {
                if(!par_b.is_deadlock(box.x - 1, box.y) ||
                   (par_b.elements[box.x - 1][box.y] == '.'))
                {
                    //Check that the player can get under the box
                    position pt(box.x + 1, box.y);
                    string path_t = par_b.find_path(par_b.player, pt);
                    if(path_t.compare("E") != 0)
                    {
                        //Create a new board and perform the movement
                        board son_b(par_b);
                        son_b.move_player(pt);
                        son_b.push_box(son_b.boxes[i], 
                                       position(box.x - 1, box.y));
                        son_b.move_player(box);
                        string new_path = par.path;
                        new_path += path_t;
                        new_path += "U";
                        to_push.push_back(node_b(son_b, new_path));
                    }
                }
            }

            //DOWN
            //Check that the box can be moved there
            if((par_b.elements[box.x + 1][box.y] != '#') &&
               (par_b.elements[box.x + 1][box.y] != '*') &&
               (par_b.elements[box.x + 1][box.y] != '$') )
            {
                if(!par_b.is_deadlock(box.x + 1, box.y) ||
                   (par_b.elements[box.x + 1][box.y] == '.'))
                {
                    //Check that the player can get over the box
                    position pt(box.x - 1, box.y);
                    string path_t = par_b.find_path(par_b.player, pt);
                    if(path_t.compare("E") != 0)
                    {
                        //Create a new board and perform the movement
                        board son_b(par_b);
                        son_b.move_player(pt);
                        son_b.push_box(son_b.boxes[i], 
                                       position(box.x + 1, box.y));
                        son_b.move_player(box);
                        string new_path = par.path;
                        new_path += path_t;
                        new_path += "D";
                        to_push.push_back(node_b(son_b, new_path));

                    }
                }
            }

            //LEFT
            //Check that the box can be moved there
            if((par_b.elements[box.x][box.y - 1] != '#') &&
               (par_b.elements[box.x][box.y - 1] != '*') &&
               (par_b.elements[box.x][box.y - 1] != '$') )
            {
                if(!par_b.is_deadlock(box.x, box.y - 1) ||
                   (par_b.elements[box.x][box.y - 1] == '.'))
                {
                    //Check that the player can get right of the box
                    position pt(box.x, box.y + 1);
                    string path_t = par_b.find_path(par_b.player, pt);
                    if(path_t.compare("E") != 0)
                    {
                        //Create a new board and perform the movement
                        board son_b(par_b);
                        son_b.move_player(pt);
                        son_b.push_box(son_b.boxes[i], 
                                       position(box.x, box.y - 1));
                        son_b.move_player(box);
                        string new_path = par.path;
                        new_path += path_t;
                        new_path += "L";
                        to_push.push_back(node_b(son_b, new_path));
                    }
                }
            }

            //RIGHT
            //Check that the box can be moved there
            if((par_b.elements[box.x][box.y + 1] != '#') &&
               (par_b.elements[box.x][box.y + 1] != '*') &&
               (par_b.elements[box.x][box.y + 1] != '$') )
            {
                if(!par_b.is_deadlock(box.x, box.y + 1) ||
                   (par_b.elements[box.x][box.y + 1] == '.'))
                {
                    //Check that the player can get left of the box
                    position pt(box.x, box.y - 1);
                    string path_t = par_b.find_path(par_b.player, pt);
                    if(path_t.compare("E") != 0)
                    {
                        //Create a new board and perform the movement
                        board son_b(par_b);
                        son_b.move_player(pt);
                        son_b.push_box(son_b.boxes[i], 
                                       position(box.x, box.y + 1));
                        son_b.move_player(box);
                        string new_path = par.path;
                        new_path += path_t;
                        new_path += "R";
                        to_push.push_back(node_b(son_b, new_path));
                    }
                }
            }

        }

        #pragma omp critical
            for(vector<node_b>::iterator it = to_push.begin(); 
                it != to_push.end();
                ++ it)
                q.push(*it);
    }

    return;
}

bool operator<(const node_b &a, const node_b &b)
{
    return (a.weight > b.weight);
}


