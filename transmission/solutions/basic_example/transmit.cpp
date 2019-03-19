#include "transmission.h"

static std::vector<bool> message;

std::vector<bool> transmit(const std::vector<bool>& data, int d)
{
    message=data;
    return message;
}
