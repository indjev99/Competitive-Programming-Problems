#pragma once

#include <array>

constexpr int NUM_REG_BITS = 8;
constexpr int NUM_REG_VALS = 1 << NUM_REG_BITS;
constexpr int NUM_REGS_IN_NUMBER = 8;
constexpr int MAX_NUM_REGS = 1e5;
constexpr int MAX_OP_DIST = 26;

using TableT = std::array<std::array<int, NUM_REG_VALS>, NUM_REG_VALS>;

void sortNumbers(int maxN);

void applyOp(int input1, int input2, int output, const TableT& opTable);
