#ifndef PARK_H_INCLUDED
#define PARK_H_INCLUDED

#include <vector>

bool get_xor(const std::vector<std::pair<int, int>>& edges);
void state_direction(const std::pair<int, int>& edge, bool direction);

void run(int n);

#endif // PARK_H_INCLUDED
