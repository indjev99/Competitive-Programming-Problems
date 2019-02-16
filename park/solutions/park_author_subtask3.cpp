#include "park.h"

const int batchSize=4;
const int qpb=3; //queries per batch
const int dirs[1<<qpb]={0b0110, 0b1111, 0b0101, 0b1010, 0b1101, 0b1011, 0b1110, 0b0111};

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
    bool xor1,xor2,xor3;
    int currDirs;
    int direction;
    while (curr<toState.size())
    {
        if (curr+batchSize>toState.size())
        {
            askAndState(toState[curr]);
            ++curr;
        }
        else
        {
            xor1=get_xor({toState[curr],toState[curr+1],toState[curr+2],toState[curr+3]});
            xor2=get_xor({toState[curr],toState[curr+3]});
            if (xor1==true) xor3=get_xor({toState[curr],toState[curr+1]});
            else xor3=get_xor({toState[curr]});
            currDirs=dirs[xor1<<2 | xor2<<1 | xor3];
            for (int i=0; i<batchSize; ++i)
            {
                state_direction(toState[curr+i],currDirs & 1<<(batchSize-1-i));
            }
            curr+=batchSize;
        }
    }
}
