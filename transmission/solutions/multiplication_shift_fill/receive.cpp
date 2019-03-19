#include "transmission.h"

static int COPIES;

static std::vector<bool> originalMessage;
static std::vector<bool> data;

static int copies(int n, int d)
{
    if (d<=1) return 1;
    if (d<=100) return 2;
    if (d<=550) return 3;
    return 4;
}

static void reconstruct_original_message(const std::vector<bool>& message)
{
    bool curr;
    int streak=0;
    for (int i=0;i<message.size();++i)
    {
        if (i==0 || message[i]!=message[i-1])
        {
            while (streak%COPIES)
            {
                originalMessage.push_back(curr);
                ++streak;
            }
            curr=message[i];
            streak=1;
        }
        else ++streak;
        originalMessage.push_back(message[i]);
    }
}
static void fill_data(int n)
{
    int m=data.size();
    int start=n/2;
    data.resize(n);
    for (int i=n-1,j=m-1;i>=n/2;--i,--j)
    {
        data[i]=data[j];
    }
}
static void reconstruct_data(int n)
{
    for (int i=0;i<originalMessage.size();i+=COPIES)
    {
        data.push_back(originalMessage[i]);
    }
    fill_data(n);
}
std::vector<bool> receive(const std::vector<bool>& message, int n, int d)
{
    COPIES=copies(n,d);
    reconstruct_original_message(message);
    reconstruct_data(n);
    return data;
}
