#ifndef BOARD_H
#define BOARD_H
#include "board.h"
#endif

#include <string>

using namespace std;

class node_b
{
    public:
        board current;
        string path;
        int weight;

        node_b(board b, string p) : current(b), path(p)
        {
            weight = 0;
            for(vector<position>::iterator it = b.boxes.begin();
                it != b.boxes.end();
                ++ it)
                weight += b.distance[(*it).x][(*it).y];

            weight += path.length() + 100 * b.empty_goals.size();
        }
};
