#include "transmission.h"

static std::vector<bool> originalMessage;
static std::vector<bool> data;

static void reconstruct_original_message(const std::vector<bool>& message)
{
    bool curr;
    int streak=0;
    for (int i=0;i<message.size();++i)
    {
        if (i==0 || message[i]!=message[i-1])
        {
            if (streak%2)
            {
                originalMessage.push_back(curr);
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
    for (int i=0;i<originalMessage.size();i+=2)
    {
        data.push_back(originalMessage[i]);
    }
    fill_data(n);
}
std::vector<bool> receive(const std::vector<bool>& message, int n, int d)
{
    reconstruct_original_message(message);
    reconstruct_data(n);
    return data;
}
