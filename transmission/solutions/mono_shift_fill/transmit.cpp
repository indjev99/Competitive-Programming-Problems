#include "transmission.h"

static std::vector<bool> message;

static void generate_message(const std::vector<bool>& data)
{
    message=data;
}
std::vector<bool> transmit(const std::vector<bool>& data, int d)
{
    generate_message(data);
    return message;
}
