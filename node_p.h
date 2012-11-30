#ifndef POSITION_H
#define POSITION_H
#include "position.h"
#endif

#include <string>

class node_p
{
    public:
        position current;
        string path;

        //Constructor to use the node with positions
        node_p(position c, string p) : current(c), path(p){};
};
