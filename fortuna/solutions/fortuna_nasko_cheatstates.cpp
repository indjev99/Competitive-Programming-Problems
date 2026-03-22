#include <utility>
#include "fortuna.h"

int n;
int m;

const int OFFSET = 16;
int val[OFFSET];

void setup(int N, int M) {
    n = N;
    m = M;

    int total = 1;
    for (int i = 0; i < OFFSET; i ++) {
        val[i] = total;
        total = total * n;
        if (total >= m) {
            int groups = total / m;
            total = total - m * groups;
        }
    }
}

std::pair<bool, int> proc(int x, int r) {
    int total_i = x % OFFSET;
    int total = val[total_i];
    int state = x / OFFSET;

    total = total * n;
    state = state * n + r;

    if (total >= m) {
        int groups = total / m;
        if (state < m * groups) {
            return {true, state / groups};
        } else {
            int rest_total = total - m * groups;
            int rest_state = state - m * groups;
            if (total_i == OFFSET - 1) {
                return {false, 0};
            }
            return {false, rest_state * OFFSET + total_i + 1};
        }
    } else {
        return {false, state * OFFSET + total_i + 1};
    }
}
