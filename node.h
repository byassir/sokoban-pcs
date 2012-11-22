#include "position.h"

#include <string>

class node
{
    public:
        position current;
        string path;

        node(position c, string p) : current(c), path(p){}
};
