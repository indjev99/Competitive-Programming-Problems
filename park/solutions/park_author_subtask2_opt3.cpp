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
    bool both=true;
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
                both=get_xor({toState[curr],toState[curr+1]});
                if (both==false)
                {
                    state_direction(toState[curr],true);
                    state_direction(toState[curr+1],true);
                    lastDir=true;
                }
                else
                {
                    lastDir=askAndState(toState[curr]);
                    lastDir=!lastDir;
                    state_direction(toState[curr+1],lastDir);
                }
                curr+=2;
            }
        }
    }
}
