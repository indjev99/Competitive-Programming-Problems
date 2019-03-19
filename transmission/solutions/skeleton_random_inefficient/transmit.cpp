#include "transmission.h"
#include <math.h>
#include <stdlib.h>

static const int SEED=0;

static std::vector<int> skeleton;
static std::vector<bool> message;

static double skeleton_fraction(int n, int d)
{
    return sqrt(d)/55;
}
static void generate_skeleton(int n, int d)
{
    srand(SEED);
    skeleton.resize(round(n*(1+skeleton_fraction(n,d))),2);
    int free=skeleton.size();
    while (free>n)
    {
        int pos=rand()%skeleton.size();
        int bit=rand()%2;
        if (skeleton[pos]==2)
        {
            skeleton[pos]=bit;
            --free;
        }
    }
}
static void generate_message(const std::vector<bool>& data)
{
    message.resize(skeleton.size());
    int j=0;
    for (int i=0;i<message.size();++i)
    {
        if (skeleton[i]==2)
        {
            message[i]=data[j];
            ++j;
        }
        else message[i]=skeleton[i];
    }
}
std::vector<bool> transmit(const std::vector<bool>& data, int d)
{
    generate_skeleton(data.size(),d);
    generate_message(data);
    return message;
}
