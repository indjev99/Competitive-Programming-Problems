#include "park.h"

std::pair<int, int> getEdge(int num)
{
    int f,t;
    f=num/2+1;
    t=f+1+num%2;
    return {f,t};
}
void run(int n)
{
    std::pair<int, int> edge;
    bool direction;
    for (int i=0; i<4*n+1; ++i)
    {
        edge=getEdge(i);
        direction=get_xor({edge});
        state_direction(edge,direction);
    }
}
