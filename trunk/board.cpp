#include "board.h"

bool box_cmp(position a, position b)
{
    if(a.x < b.x)
        return true;

    if(a.x == b.x)
        if(a.y < b.y)
            return true;

    return false;
}

board::board(const board &b)
{
    num_rows = b.num_rows;
    num_cols = b.num_cols;

    elements = new char*[num_rows];
    for(int i = 0; i < num_rows; ++ i)
    {
        elements[i] = new char[num_cols];
        for(int j = 0; j < num_cols; ++ j)
            elements[i][j] = b.elements[i][j];
    }

    s_deadlock = new bool*[num_rows];
    for(int i = 0; i < num_rows; ++ i)
    {
        s_deadlock[i] = new bool[num_cols];
        for(int j = 0; j < num_cols; ++ j)
            s_deadlock[i][j] = b.s_deadlock[i][j];
    }

    distance = new int*[num_rows];
    for(int i = 0; i < num_rows; ++ i)
    {
        distance[i] = new int[num_cols];
        for(int j = 0; j < num_cols; ++ j)
            distance[i][j] = b.distance[i][j];
    }

    player = position(b.player);

    vector<position>::iterator it;

    for(int i = 0; i < b.boxes.size(); ++ i)
        boxes.push_back(position(b.boxes[i]));
    
    for(int i = 0; i < b.empty_goals.size(); ++ i)
        empty_goals.push_back(position(b.empty_goals[i]));
}

board::board(vector<string> lines)
{
    this->num_rows = lines.size();
    this->num_cols = lines.back().size();

    this->elements = new char*[this->num_rows];
    for(int i = 0; i < lines.size(); ++ i)
        this->elements[i] = new char[this->num_cols];

    int i = 0;
    vector<string>::iterator it;
    for(it = lines.begin(); it != lines.end(); ++ it)
    {
        for(int j = 0; j < this->num_cols; ++ j)
        {
            //Add the element to the array
            char el = (*it)[j];
            this->elements[i][j] = el;

            //If it is a special character, find out what it is
            switch(el)
            {
                case '@':
                case '+':
                    this->player = position(i, j);
                    break;
                case '$':
                case '*':
                    (this->boxes).push_back(position(i, j));
                    break;
                case '.':
                    (this->empty_goals).push_back(position(i, j));
                    break;
                default:
                    break;
            }
        }
        ++ i;
    }

    distance = new int*[num_rows];
    for(int i = 0; i < num_rows; ++ i)
        distance[i] = new int[num_cols];

    update_distances();
    find_deadlocks();
}

void board::print()
{
    for(int i = 0; i < num_rows; ++ i)
    {
        for(int j = 0; j < num_cols; ++ j)
            cout << elements[i][j];
        cout << endl;
    }
    return;
}

bool board::move_player(position p)
{
    if((elements[p.x][p.y] == '#') || 
       (elements[p.x][p.y] == '*') ||
       (elements[p.x][p.y] == '$'))
    {
        cout << "Trying to move player into an invalid position: ";
        p.print();
        this->print();
        return false;
    }

    //Update the position the player is leaving
    if(elements[player.x][player.y] == '@')
    {
        elements[player.x][player.y] = ' ';
    }else if(elements[player.x][player.y] == '+')
    {
        elements[player.x][player.y] = '.';
    }else
    {
        cout << "Player disappeared o.o" << endl;
    }

    //Place the player in the new position
    if(elements[p.x][p.y] == '.')
    {
        elements[p.x][p.y] = '+';
    }else
    {
        elements[p.x][p.y] = '@';
    }

    //Update the player's position
    player.x = p.x;
    player.y = p.y;
    return true;
}

bool board::push_box(position &box, position dest)
{
    bool update_dist = false;
    if((elements[dest.x][dest.y] == '#') || 
       (elements[dest.x][dest.y] == '*') ||
       (elements[dest.x][dest.y] == '$') ||
       (elements[dest.x][dest.y] == '@') ||
       (elements[dest.x][dest.y] == '+'))
    {
        cout << "Trying to move box into an invalid position" << endl;
        return false;
    }

    //Update the position where the box came from   
    if(elements[box.x][box.y] == '*')
    {
        elements[box.x][box.y] = '.';
        empty_goals.push_back(position(box));
        update_dist = true;
    }else if(elements[box.x][box.y] == '$')
    {
        elements[box.x][box.y] = ' ';
    }else
    {
        cout << "Box disappeared o.o" << endl;
        return false;
    }

    //Update the destination
    if(elements[dest.x][dest.y] == '.')
    {
        elements[dest.x][dest.y] = '*';
        vector<position>::iterator it;
        for(it = empty_goals.begin(); it != empty_goals.end(); ++ it)
            if((*it).equals(dest))
            {
                empty_goals.erase(it);
                break; 
            }
        update_dist = true;
    }else
    {
        elements[dest.x][dest.y] = '$';
    }

    //Update the box's attributes
    box.x = dest.x;
    box.y = dest.y;
    if(update_dist)
        update_distances();
    return true;
}

string board::find_path(position a, position b)
{
    queue<node_p> q;
    bool visited[num_rows][num_cols];

    memset(visited, 0, num_rows * num_cols * sizeof(bool));
    //Don't do the search if b is an invalid position to move to
    if((elements[b.x][b.y] == '$') ||
       (elements[b.x][b.y] == '*') ||
       (elements[b.x][b.y] == '#'))
        return "E";

    //Add the first element to the queue
    q.push(node_p(a, ""));

    while(!q.empty())
    {
        //Take the first element
        node_p par = q.front();
        q.pop();

        position p_pos = par.current;
        //If it is in the destination, stop the search
        if(p_pos.equals(b))
            return par.path;

        //If this node was already visited, don't check again
        if(visited[p_pos.x][p_pos.y])
            continue;

        //Otherwise, mark as visited and analyze possible sons to add to the 
        //queue

        visited[p_pos.x][p_pos.y] = true;

        //Top
        if((elements[p_pos.x - 1][p_pos.y] != '*') &&
           (elements[p_pos.x - 1][p_pos.y] != '$') &&
           (elements[p_pos.x - 1][p_pos.y] != '#'))
        {
            string son_p = par.path;
            son_p.append("U");

            q.push(node_p(
                     position(p_pos.x - 1, p_pos.y), 
                     son_p)
                  );
        }
        //Bottom
        if((elements[p_pos.x + 1][p_pos.y] != '*') &&
           (elements[p_pos.x + 1][p_pos.y] != '$') &&
           (elements[p_pos.x + 1][p_pos.y] != '#'))
        {
            string son_p = par.path;
            son_p.append("D");

            q.push(node_p(
                      position(p_pos.x + 1, p_pos.y), 
                      son_p)
                  );
        }
        //Left
        if((elements[p_pos.x][p_pos.y - 1] != '*') &&
           (elements[p_pos.x][p_pos.y - 1] != '$') &&
           (elements[p_pos.x][p_pos.y - 1] != '#'))
        {
            string son_p = par.path;
            son_p.append("L");

            q.push(node_p(
                     position(p_pos.x, p_pos.y - 1), 
                     son_p)
                  );
        }
        //Right
        if((elements[p_pos.x][p_pos.y + 1] != '*') &&
           (elements[p_pos.x][p_pos.y + 1] != '$') &&
           (elements[p_pos.x][p_pos.y + 1] != '#'))
        {
            string son_p = par.path;
            son_p.append("R");

            q.push(node_p(
                      position(p_pos.x, p_pos.y + 1),
                      son_p)
                  );
        }

        //delete &par;
    }

    //If the iterations finish without returning, there is no possible path
    return "E";
}

void board::find_deadlocks()
{
    //Allocate a new static deadlock matrix
    s_deadlock = new bool*[num_rows];
    for(int i = 0; i < num_rows; ++ i)
    {
        s_deadlock[i] = new bool[num_cols];
        for(int j = 0; j < num_cols; ++ j)
            s_deadlock[i][j] = false;
    }

    //Go through all whitespaces of the board and identify corners
    for(int i = 1; i < num_rows - 1; ++ i)
    {
        for(int j = 1; j < num_cols - 1; ++ j)
        {
            //Don't double check positions
            if(s_deadlock[i][j])
                continue;
            //Only check for deadlocks on white spaces
            if((elements[i][j] == ' ') ||
               (elements[i][j] == '$') ||
               (elements[i][j] == '@')
               )
            {
                //Check if this space is surrounded in both directions
                int num_walls = 0;
                if((elements[i - 1][j] == '#') || (elements[i + 1][j] == '#'))
                    ++ num_walls;
                if(elements[i][j - 1] == '#' || (elements[i][j + 1] == '#'))
                    ++ num_walls;

                if(num_walls > 1)
                {
                    s_deadlock[i][j] = true;
                    //Check the whole row and column to see if this is a 
                    //corridor
                    int factor = 0;
                    //Vertical check
                    if(elements[i - 1][j] == '#')
                    {
                        factor = 1;
                    }else
                    {
                        factor = -1;
                    }
                    
                    int k = i;
                    int num_jumps = 0;
                    bool is_corr = false;
                    while((0 < k) && (k < num_rows - 1) &&
                            ((elements[k][j + 1] == '#') || 
                            (elements[k][j - 1] == '#'))
                         )
                    {
                        k += factor;
                        if(elements[k][j] == '#')
                        {
                            is_corr = true;
                            break;
                        }else if((elements[k][j] == '.') ||
                                 (elements[k][j] == '+'))
                        {
                            break;
                        }
                        ++ num_jumps;
                    }

                    if(is_corr)
                    {
                        k = i;
                        while(num_jumps > 0)
                        {
                            k += factor;
                            -- num_jumps;
                            //Don't mark goals as deadlocks
                            if((elements[k][j] == '.') ||
                               (elements[k][j] == '*') ||
                               (elements[k][j] == '+'))
                                continue;
                            s_deadlock[k][j] = true;
                        }
                    }

                    //Horizontal check
                    if(elements[i][j - 1] == '#')
                    {
                        factor = 1;
                    }else
                    {
                        factor = -1;
                    }
                    
                    k = j;
                    num_jumps = 0;
                    is_corr = false;
                    while((0 < k) && (k < num_cols - 1) &&
                           ((elements[i + 1][k] == '#') || 
                           (elements[i - 1][k] == '#'))
                         )
                    {
                        k += factor;
                        if(elements[i][k] == '#')
                        {
                            is_corr = true;
                            break;
                        }else if((elements[i][k] == '.') ||
                                 (elements[i][k] == '+'))
                        {
                            break;
                        }

                        ++ num_jumps;
                    }

                    if(is_corr)
                    {
                        k = j;
                        while(num_jumps > 0)
                        {
                            k += factor;
                            -- num_jumps;
                            //Don't mark goals as deadlocks
                            if((elements[i][k] == '.') ||
                               (elements[i][k] == '*') ||
                               (elements[i][k] == '+'))
                                continue;
                            s_deadlock[i][k] = true;
                        }
                    }
                }
            }
        }
    }

    return;
}

bool board::is_deadlock(int x, int y)
{
    if(s_deadlock[x][y])
        return true;

    bool **checked = new bool*[num_rows];
    for(int i = 0; i < num_rows; ++ i)
    {
        checked[i] = new bool[num_cols];
        for(int j = 0; j < num_cols; ++ j)
            checked[i][j] = false;
    }

    return dynamic_deadlock(x, y, checked);
}

bool board::dynamic_deadlock(int x, int y, bool **checked)
{
    bool down_dead = false;
    bool up_dead = false;
    bool left_dead = false;
    bool right_dead = false;

    //Check if the box can leave through bottom
    if((elements[x + 1][y] == '$') || (elements[x + 1][y] == '*'))
    {
        if(!checked[x + 1][y])
        {
            checked[x + 1][y] = true;
            down_dead = dynamic_deadlock(x + 1, y, checked);
            checked[x + 1][y] = false;
        }else
        {
            down_dead = true;
        }
    }else if(elements[x + 1][y] == '#')
    {
        down_dead = true;
    }

    //Check if the box can leave through top
    if((elements[x - 1][y] == '$') || (elements[x - 1][y] == '*'))
    {
        if(!checked[x - 1][y])
        {
            checked[x - 1][y] = true;
            up_dead = dynamic_deadlock(x - 1, y, checked);
            checked[x - 1][y] = false;
        }else
        {
            up_dead = true;
        }
    }else if(elements[x - 1][y] == '#')
    {
        up_dead = true;
    }

    //Check if the box can leave through left
    if((elements[x][y - 1] == '$') || (elements[x][y - 1] == '*'))
    {
        if(!checked[x][y - 1])
        {
            checked[x][y - 1] = true;
            left_dead = dynamic_deadlock(x, y - 1, checked);
            checked[x][y - 1] = false;
        }else
        {
            left_dead = true;
        }
    }else if(elements[x][y - 1] == '#')
    {
        left_dead = true;
    }

    //Check if the box can leave through right
    if((elements[x][y + 1] == '$') || (elements[x][y + 1] == '*'))
    {
        if(!checked[x][y + 1])
        {
            checked[x][y + 1] = true;
            right_dead = dynamic_deadlock(x, y + 1, checked);
            checked[x][y + 1] = false;
        }else
        {
            right_dead = true;
        }
    }else if(elements[x][y + 1] == '#')
    {
        right_dead = true;
    }

    //After determining the escaping points, if the box is cornered, then it's
    //in dynamic deadlock
    if((down_dead && left_dead) ||
       (down_dead && right_dead) ||
       (up_dead && left_dead) ||
       (up_dead && right_dead))
        return true;

    return false;
}

void board::update_distances()
{
    for(int i = 0; i < num_rows; ++ i)
        for(int j = 0; j < num_cols; ++ j)
        {
            int d = INT_MAX;
            for(vector<position>::iterator it = empty_goals.begin();
                it != empty_goals.end();
                ++ it)
            {
                int d_goal = abs((*it).x - i) + abs((*it).y - j);
                if(d_goal < d)
                    d = d_goal;
            }
            distance[i][j] = d;
        }
    return;
}

string board::get_key()
{
    position reach = find_area();
    sort(boxes.begin(), boxes.end(), box_cmp);

    stringstream key;
    key << reach.x << "," << reach.y << ";";
    for(vector<position>::iterator it = boxes.begin(); it != boxes.end(); ++ it)
        key << (*it).x << "," << (*it).y << ";";

    return key.str();
}

position board::find_area()
{
    position min = position(num_rows, num_cols);
    queue<position> q;
    bool visited[num_rows][num_cols];

    for(int i = 0; i < num_rows; ++ i)
        for(int j = 0; j < num_cols; ++ j)
            visited[i][j] = false;

    q.push(player);

    while(!q.empty())
    {
        position par = q.front();
        q.pop();

        int x = par.x;
        int y = par.y;

        if(visited[x][y])
            continue;

        visited[x][y] = true;

        if(box_cmp(par, min))
        {
            min.x = x;
            min.y = y;
        }

        if((min.x == 1) && (min.y == 1))
            break;

        //Add the sons to the queue

        //Top
        if((elements[x - 1][y] == ' ') ||
           (elements[x - 1][y] == '.') ||
           (elements[x - 1][y] == '+')||
           (elements[x - 1][y] == '@'))
            q.push(position(x - 1, y));

        //Bottom
        if((elements[x + 1][y] == ' ') ||
           (elements[x + 1][y] == '.') ||
           (elements[x + 1][y] == '+')||
           (elements[x + 1][y] == '@'))
            q.push(position(x + 1, y));

        //Left
        if((elements[x][y - 1] == ' ') ||
           (elements[x][y - 1] == '.') ||
           (elements[x][y - 1] == '+')||
           (elements[x][y - 1] == '@'))
            q.push(position(x, y - 1));

        //Right
        if((elements[x][y + 1] == ' ') ||
           (elements[x][y + 1] == '.') ||
           (elements[x][y + 1] == '+')||
           (elements[x][y + 1] == '@'))
            q.push(position(x, y + 1));

    }
    return min;
}
