#include <utility>
#include "fortuna.h"

int n;
int m;
const int MAX_N = 30;
int code[MAX_N][MAX_N];
std::pair<int, int> inv[MAX_N * MAX_N];

void setup(int N, int M) {
    n = N;
    m = M;

    int now = 0;
    for (int i = 1; i < m; i ++) {
        for (int j = 0; j < i; j ++) {
            code[i][j] = now;
            inv[now] = {i, j};
            now ++;
        }
    }
}

std::pair<bool, int> proc(int x, int r) {
    int total = inv[x].first;
    int state = inv[x].second;

    total = total * n;
    state = state * n + r;

    if (total >= m) {
        int groups = total / m;
        if (state < m * groups) {
            return {true, state / groups};
        } else {
            int rest_total = total - m * groups;
            int rest_state = state - m * groups;
            return {false, code[rest_total][rest_state]};
        }
    } else {
        return {false, code[total][state]};
    }
}
