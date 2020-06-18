#include <iostream>
#include <stdlib.h>
using namespace std;

const int SEED=0;

const int MAX_V=1e9-1;

//Subtask for 91 and 100
const int MAX_POW=17;
const int START_X=2e5;
const int MAX_N=1e6;
const int MAX_M=1e6;

/*//Subtask for 72
const int MAX_POW=14;
const int START_X=2e5;
const int MAX_N=1e4;
const int MAX_M=1e4;*/

int main()
{
    srand(SEED);
    int y=2e5;
    cout<<START_X+(1<<MAX_POW-1)-1<<'\n';
    cout<<MAX_N<<' '<<MAX_M<<'\n';
    for (int i=1;i<MAX_POW;++i)
    {
        for (int j=START_X+(1<<i-1);j<START_X+(1<<MAX_POW);j+=2<<i)
        {
            cout<<j<<' '<<j+(1<<i)-1<<' '<<y<<' '<<MAX_V<<'\n';
            if (rand()%2) --y;
        }
        --y;
    }
    y=2e5+10;
    for (int i=(1<<MAX_POW-1)-1;i<MAX_N;++i)
    {
        cout<<5<<' '<<1e6-1<<' '<<(y++)<<' '<<rand()%5+1<<'\n';
    }
    for (int i=0;i<MAX_M;++i)
    {
        cout<<START_X+rand()%(1<<MAX_POW)<<' '<<(rand()*(RAND_MAX+1)+rand())%((int)1e6-5)+5<<'\n';
    }
}

