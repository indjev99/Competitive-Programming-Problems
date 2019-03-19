#include "transmission.h"
#include <array>
#include <math.h>
#include <stdlib.h>

static const int SEED=11111;
static const int MAX_D=1000;

static std::vector<int> skeleton;
static std::vector<std::array<bool,MAX_D+1>> makeDeletion;
static std::vector<bool> deletions;
static std::vector<bool> originalMessage;
static std::vector<bool> data;

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
static bool make_deletion(const std::vector<bool>& message, int i, int j)
{
    if (j>0 && (skeleton[i-1]==2 || skeleton[i-1]==message[j-1])) return 0;
    else return 1;
}
static void find_deletions(const std::vector<bool>& message, int d)
{
    deletions.resize(skeleton.size());
    int i=skeleton.size();
    int j=message.size();
    while (i)
    {
        deletions[i-1]=make_deletion(message,i,j);
        if (deletions[i-1]==0) --j;
        --i;
    }
}
static void reconstruct_original_message(const std::vector<bool>& message)
{
    originalMessage.resize(skeleton.size());
    int j=0;
    for (int i=0;i<originalMessage.size();++i)
    {
        if (!deletions[i])
        {
            originalMessage[i]=message[j];
            ++j;
        }
        else originalMessage[i]=rand()%2;
    }
}
static void reconstruct_data(int n)
{
    data.resize(n);
    int j=0;
    for (int i=0;i<n;++i)
    {
        while (skeleton[j]!=2) ++j;
        data[i]=originalMessage[j];
        ++j;
    }
}
std::vector<bool> receive(const std::vector<bool>& message, int n, int d)
{
    generate_skeleton(n,d);
    find_deletions(message,d);
    reconstruct_original_message(message);
    reconstruct_data(n);
    return data;
}
