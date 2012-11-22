#include "simulate.h"

void run_simulation(board b, string s)
{
    string::iterator it;
    char **board = b.elements;
    position curr(b.player);
    cout << "Solution found: " << s << endl;
    cout << "Simulation starting in 5 seconds." << endl;
    sleep(5);
    b.print();
    for(it = s.begin(); it !=s.end(); ++ it)
    {
        usleep(500000);
        char move = *it;
        position cp(curr);
        switch(move){
            case 'U':
                -- curr.x;
                --cp.x;
                if((board[curr.x][curr.y] == '*') ||
                   (board[curr.x][curr.y] == '$'))
                    b.push_box(cp,
                               position(curr.x - 1, curr.y));
                break;
            case 'D':
                ++ curr.x;
                ++ cp.x;
                if((board[curr.x][curr.y] == '*') ||
                   (board[curr.x][curr.y] == '$'))
                    b.push_box(cp,
                               position(curr.x + 1, curr.y));
                break;
            case 'L':
                -- curr.y;
                -- cp.y;
                if((board[curr.x][curr.y] == '*') ||
                   (board[curr.x][curr.y] == '$'))
                    b.push_box(cp,
                               position(curr.x, curr.y - 1));
                break;
            case 'R':
                ++ curr.y;
                ++ cp.y;
                if((board[curr.x][curr.y] == '*') ||
                   (board[curr.x][curr.y] == '$'))
                    b.push_box(cp,
                               position(curr.x, curr.y + 1));
                break;
        }
        b.move_player(curr);
        b.print();
    }
}
