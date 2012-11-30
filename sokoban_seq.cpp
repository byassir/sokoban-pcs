#include "sokoban.h"

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        cout << "Usage: ./sokoban_seq input_file" << endl;
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

    b.print();

    string sol = push_to_goals(b);
    cout << sol << endl;
//    run_simulation(b, sol);
    return 1;
}

void call_print(position p)
{
    cout << "\t";
    p.print();
}

bool operator<(const node_b &a, const node_b &b)
{
    return (a.weight > b.weight);
}

string push_to_goals(board in)
{
    set<string> visited;
    priority_queue<node_b> q;
    q.push(node_b(in, ""));

    while(!q.empty())
    {
        node_b par = q.top();
        q.pop();
        board par_b = par.current;

        //Check if this is a final solution
        if(par_b.empty_goals.size() == 0)
            return par.path;

        //Check if this element has already been visited
        if(!visited.insert(par_b.get_key()).second)
            continue;

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
                        q.push(node_b(son_b, new_path));
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
                        q.push(node_b(son_b, new_path));
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
                        q.push(node_b(son_b, new_path));
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
                        q.push(node_b(son_b, new_path));
                    }
                }
            }

        }
    }

    //If here, then no valid solution was found
    return "E";
}
