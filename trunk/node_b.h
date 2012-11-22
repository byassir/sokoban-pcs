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

        node_b(board b, string p) : current(b), path(p){}
};
