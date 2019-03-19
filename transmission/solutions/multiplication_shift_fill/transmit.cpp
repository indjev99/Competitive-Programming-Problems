#include "transmission.h"

static int COPIES;

static std::vector<bool> message;

static int copies(int n, int d)
{
    if (d<=1) return 1;
    if (d<=100) return 2;
    if (d<=550) return 3;
    return 4;
}

static void generate_message(const std::vector<bool>& data)
{
    for (int i=0;i<data.size();++i)
    {
        for (int j=0;j<COPIES;++j)
        {
            message.push_back(data[i]);
        }
    }
}
std::vector<bool> transmit(const std::vector<bool>& data, int d)
{
    COPIES=copies(data.size(),d);
    generate_message(data);
    return message;
}
