#include "board.h"

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
    }else
    {
        elements[dest.x][dest.y] = '$';
    }

    //Update the box's attributes
    box.x = dest.x;
    box.y = dest.y;
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

bool board::operator<(const board b) const
{
    if((num_rows != b.num_rows) ||
        num_cols != b.num_cols)
        return true;

    for(int i = 0; i < num_rows; ++ i)
        for(int j = 0; j < num_cols; ++ j)
            if(elements[i][j] != b.elements[i][j])
                return true;

    return false;
}
