#ifndef TRANSMISSION_H_INCLUDED
#define TRANSMISSION_H_INCLUDED

#include <vector>

std::vector<bool> transmit(const std::vector<bool>& data, int d);
std::vector<bool> receive(const std::vector<bool>& message, int n, int d);

#endif // TRANSMISSION_H_INCLUDED
