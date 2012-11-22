#include <iostream>

using namespace std;

class position
{
    public:
        int x;
        int y;

        position(int x0, int y0) : x(x0), y(y0){}
        position() : x(-1), y(-1){}
        void print();

        //Tests against the given parameter for equality
        bool equals(position p);
};
