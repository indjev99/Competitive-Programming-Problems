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
    state_direction(getEdge(-1),true);
    for (int i=0; i<4*n+1; ++i)
    {
        edge=getEdge(i);
        if (i%2==1 || i==0 || i==4*n) direction=true;
        else direction=get_xor({edge});
        state_direction(edge,direction);
    }
}
