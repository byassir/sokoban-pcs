#include "simulate.h"

void run_simulation(board b, string s)
{
    string::iterator it;
    char **board = b.elements;
    position curr(b.player);
    b.print();
    for(it = s.begin(); it !=s.end(); ++ it)
    {
        usleep(500000);
        char move = *it;
        switch(move){
            case 'U':
                -- curr.x;
                if((board[curr.x][curr.y] == '*') ||
                   (board[curr.x][curr.y] == '$'))
                    b.push_box(*(new position(curr)),
                               *(new position(curr.x - 1, curr.y)));
                break;
            case 'D':
                ++ curr.x;
                if((board[curr.x][curr.y] == '*') ||
                   (board[curr.x][curr.y] == '$'))
                    b.push_box(*(new position(curr)),
                               *(new position(curr.x + 1, curr.y)));
                break;
            case 'L':
                -- curr.y;
                if((board[curr.x][curr.y] == '*') ||
                   (board[curr.x][curr.y] == '$'))
                    b.push_box(*(new position(curr)),
                               *(new position(curr.x, curr.y - 1)));
                break;
            case 'R':
                ++ curr.y;
                if((board[curr.x][curr.y] == '*') ||
                   (board[curr.x][curr.y] == '$'))
                    b.push_box(*(new position(curr)),
                               *(new position(curr.x, curr.y + 1)));
                break;
        }
        b.move_player(curr);
        b.print();
    }
}
