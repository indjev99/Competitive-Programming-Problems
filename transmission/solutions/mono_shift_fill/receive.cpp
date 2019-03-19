#include "transmission.h"

static std::vector<bool> data;

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
static void reconstruct_data(const std::vector<bool>& message, int n)
{
    data=message;
    fill_data(n);
}
std::vector<bool> receive(const std::vector<bool>& message, int n, int d)
{
    reconstruct_data(message,n);
    return data;
}
