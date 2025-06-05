#pragma once

#include <vector>

void solve(int n, int m, int k, const std::vector<std::vector<bool>>& blocked);

void goDeeper(int s);
bool investigate(int s);
