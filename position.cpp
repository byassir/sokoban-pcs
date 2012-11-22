#include "position.h"

void position::print()
{
    cout << "(" << this->x << ", " << this->y << ")" << endl;
}

bool position::equals(position p)
{
    return ((this->x == p.x) && (this->y == p.y));
}
