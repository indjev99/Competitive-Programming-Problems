#include "transmission.h"
#include <math.h>
#include <stdlib.h>

static const int SEED=11111;

static std::vector<int> skeleton;
static std::vector<bool> message;

static double skeleton_fraction(int n, int d)
{
    return sqrt(d)/53;
}
static void generate_skeleton(int n, int d)
{
    srand(SEED);
    int steps[4];
    int step4=round(4+4/skeleton_fraction(n,d));
    steps[0]=(step4+3)/4;
    steps[2]=(step4+2)/4;
    steps[3]=(step4+1)/4;
    steps[1]=step4/4;
    int curr=0;
    int next=0;
    int free=0;
    int i=0;
    while (free<n)
    {
        if (i==next)
        {
            skeleton.push_back(rand()%2);
            next+=steps[curr];
            curr=(curr+1)%4;
        }
        else
        {
            skeleton.push_back(2);
            ++free;
        }
        ++i;
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
