#include <utility>
#include "fortuna.h"

int n;
int m;
int LOG;

void setup(int N, int M) {
    n = N;
    m = M;
    LOG = 0;

    int pw = 1;
    while (pw < m) { pw *= n; LOG ++; }
}

std::pair<bool, int> proc(int x, int r) {
    int log = x % LOG;
    int total = 1;
    for (int i = 0; i < log; i ++) { total *= n; }
    int state = x / LOG;

    total *= n;
    log ++;
    state = state * n + r;

    if (total >= m) {
        int groups = total / m;
        if (state < m * groups) {
            return {true, state / groups};
        } else {
            int rest = total - m * groups;
            int state_rest = state - m * groups;
            int nw = 1;
            int nwlog = 0;
            while (nw <= rest) {
                nw *= n;
                nwlog ++;
            }
            nw /= n;
            nwlog --;
            if (state_rest < nw) {
                return {false, state_rest * LOG + nwlog};
            } else {
                return {false, 0};
            }
        }
    } else {
        return {false, state * LOG + log};
    }
}
