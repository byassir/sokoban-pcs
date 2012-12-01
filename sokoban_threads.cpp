#include "sokoban.h"
#include <thread>

//Global variables used for sharing
int num_threads;

//Here we will save the solution. This must be protected against writing
mutex solution_mutex;
condition_variable solution_cv;
string solution;

//This is the queue containing the work to be done. Access to it must be
//restricted
mutex q_mutex;
priority_queue<node_b> q;

//Set containing the elements that have been analyzed already. Access to it
//must be restricted
mutex visited_mutex;
set<string> visited;

int main(int argc, char **argv)
{
    if(argc < 3)
    {
        cout << "Usage: ./sokoban_seq input_file num_threads" << endl;
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

    //Read the number of threads
    stringstream ss;
    ss << argv[2];
    ss >> num_threads;

//    b.print();

    string sol = push_to_goals(b);
    cout << sol << endl;
//    run_simulation(b, sol);
    return 1;
}

string push_to_goals(board in)
{
    //Add some work to do
    q.push(node_b(in, ""));

    //Acquire a unique lock over the solution in order to wait for the condition
    //variable and initialize the solution
    unique_lock<mutex> lock(solution_mutex);
    solution = "E";

    //Spawn all the workers
    thread workers[num_threads];
    for(int i = 0; i < num_threads; ++ i)
        workers[i] = thread(analyze);

    //Wait until a thread finds a solution
    solution_cv.wait(lock);

    //Join with the exiting threads
    for(int i = 0; i < num_threads; ++ i)
        workers[i].join();

    return solution;
}

void analyze()
{
    //Iterate while there is no solution found
    while(solution.compare("E") == 0)
    {
        vector<node_b> to_push;
        //For short critical section, si spin waiting.
        do{}while(!q_mutex.try_lock());
        //While there is nothing to consume, just loop
        if(q.empty())
        {
            q_mutex.unlock();
            continue;
        }
        node_b par = q.top();
        q.pop();
        q_mutex.unlock();
        board par_b = par.current;

        //Check if this is a final solution
        if(par_b.empty_goals.size() == 0)
        {
            solution_mutex.lock();
            solution = par.path;
            solution_mutex.unlock();
            solution_cv.notify_one();
            return;
        }

        //Check if this element has already been visited
        visited_mutex.lock();
        if(!visited.insert(par_b.get_key()).second)
        {
            visited_mutex.unlock();
            continue;
        }
        visited_mutex.unlock();

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

        q_mutex.lock();
        for(vector<node_b>::iterator it = to_push.begin(); 
            it != to_push.end();
            ++ it)
            q.push(*it);
        q_mutex.unlock();
    }

    return;
}

bool operator<(const node_b &a, const node_b &b)
{
    return (a.weight > b.weight);
}


