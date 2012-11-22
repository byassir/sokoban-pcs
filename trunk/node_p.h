#include "position.h"

#include <string>

class node_p
{
    public:
        position current;
        string path;

        //Constructor to use the node with positions
        node_p(position c, string p) : current(c), path(p){};
};
