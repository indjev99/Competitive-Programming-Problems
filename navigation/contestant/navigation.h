#include <vector>

constexpr int INIT_COLOR = 0;
constexpr int MAX_COLORS = 1500;
constexpr int TERMINATE_CODE = -1;
constexpr int MAX_ITERS = 3000;

std::pair<int, int> navigate(int currColor, std::vector<int> adjColors);
