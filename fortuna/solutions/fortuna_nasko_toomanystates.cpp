#include <utility>
#include "fortuna.h"

int n;
int m;

void setup(int N, int M) {
    n = N;
    m = M;
}

std::pair<bool, int> proc(int x, int r) {
    int total = x % m + 1;
    int state = x / m;

    total = total * n;
    state = state * n + r;

    if (total >= m) {
        int groups = total / m;
        if (state < m * groups) {
            return {true, state / groups};
        } else {
            int rest_total = total - m * groups;
            int rest_state = state - m * groups;
            return {false, rest_state * m + (rest_total - 1)};
        }
    } else {
        return {false, state * m + (total - 1)};
    }
}
