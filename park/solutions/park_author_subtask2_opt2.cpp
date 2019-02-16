#include "park.h"

std::pair<int, int> getEdge(int num)
{
    int f,t;
    f=num/2+1;
    t=f+1+num%2;
    return {f,t};
}
bool askAndState(const std::pair<int, int>& edge)
{
    bool direction=get_xor({edge});
    state_direction(edge,direction);
    return direction;
}
std::vector<std::pair<int, int>> toState;
void run(int n)
{
    std::pair<int, int> edge;
    for (int i=0; i<4*n+1; ++i)
    {
        edge=getEdge(i);
        if (i%2==1 || i==0 || i==4*n) state_direction(edge,true);
        else toState.push_back(edge);
    }
    int curr=0;
    bool lastDir=true;
    while (curr<toState.size())
    {
        if (lastDir==false)
        {
            state_direction(toState[curr],true);
            lastDir=true;
            ++curr;
        }
        else
        {
            if (curr+1==toState.size())
            {
                askAndState(toState[curr]);
                ++curr;
            }
            else
            {
                lastDir=askAndState(toState[curr+1]);
                if (lastDir==false) state_direction(toState[curr],true);
                else askAndState(toState[curr]);
                curr+=2;
            }
        }
    }
}
