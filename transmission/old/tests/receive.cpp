#include "transmission.h"
#include <array>
#include <math.h>
#include <stdlib.h>

static const int SEED=0;
static const int MAX_D=1000;

static std::vector<int> skeleton;
static std::vector<std::array<bool,MAX_D+1>> makeDeletion;
static std::vector<bool> deletions;
static std::vector<bool> originalMessage;
static std::vector<bool> data;

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
static void min_edit_dist_deletions(const std::vector<bool>& message, int d)
{
    makeDeletion.resize(skeleton.size()+1);
    for (int i=0;i<=skeleton.size();++i)
    {
        for (int j=std::max(0,i-d);j<=message.size() && j<=i;++j)
        {
            if (j>0 && (skeleton[i-1]==2 || skeleton[i-1]==message[j-1])) makeDeletion[i][i-j]=0;
            else makeDeletion[i][i-j]=1;
        }
    }
}
static void find_deletions(const std::vector<bool>& message, int d)
{
    min_edit_dist_deletions(message,d);
    deletions.resize(skeleton.size());
    int i=skeleton.size();
    int j=message.size();
    while (i)
    {
        deletions[i-1]=makeDeletion[i][i-j];
        if (makeDeletion[i][i-j]==0) --j;
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
