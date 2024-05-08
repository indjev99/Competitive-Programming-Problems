#include <vector>

constexpr int EMPTY = 0;
constexpr int HOR_SINGLE = -1;
constexpr int VERT_SINGLE = -2;
constexpr int HOR_DOUBLE = -3;
constexpr int VERT_DOUBLE = -4;

void init(int numSubtests, double timeLimit);
bool solve(std::vector<std::vector<int>>& puzzle);

double timePassed();
