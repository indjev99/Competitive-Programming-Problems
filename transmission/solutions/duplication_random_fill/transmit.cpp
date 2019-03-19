#include "transmission.h"

static std::vector<bool> message;

static void generate_message(const std::vector<bool>& data)
{
    for (int i=0;i<data.size();++i)
    {
        message.push_back(data[i]);
        message.push_back(data[i]);
    }
}
std::vector<bool> transmit(const std::vector<bool>& data, int d)
{
    generate_message(data);
    return message;
}
