#include "fortuna.h"

const int MAX_N = 30;
const int MAX_M = 30;
const int MAX_CODES = MAX_M * (MAX_M + 1) / 2;

int n;
int m;

double cost[MAX_M];
int move[MAX_M];

int currCode;
int codesStart[MAX_M];
std::pair<int, int> codeToCurrVal[MAX_CODES];
std::pair<bool, int> codeToRet[MAX_CODES][MAX_N];

void encodeDfs(int curr)
{
    if (codesStart[curr] >= 0) return;

    if (move[curr])
    {
        encodeDfs(move[curr]);
        encodeDfs(curr - move[curr]);
    }

    codesStart[curr] = currCode;
    currCode += curr;

    for (int val = 0; val < curr; val++)
    {
        codeToCurrVal[codesStart[curr] + val] = {curr, val};
    }

    int next = curr * n % m;
    if (next == 0) return;
    encodeDfs(next);
}

int getCode(int curr, int val)
{
    if (move[curr] == 0)
    {
        return codesStart[curr] + val;
    }

    if (val < move[curr])
    {
        return getCode(move[curr], val);
    }
    else
    {
        return getCode(curr - move[curr], val - move[curr]);
    }
}

void setup(int _n, int _m)
{
    n = _n;
    m = _m;

    double baseCost;
    {
        int curr = 1;
        int logCeil = 0;
        while (curr < m)
        {
            curr *= n;
            logCeil++;
        }
        int d = curr / m;
        int succ = d * m;
        baseCost = (double) logCeil * curr / succ;
    }

    for (int i = 1; i < m; i++)
    {
        cost[i] = baseCost;
        move[i] = 0;
    }

    for (int t = 0; t < 1000; t++)
    {
        for (int i = 1; i < m; i++)
        {
            {
                int next = i * n;
                double newCost;
                if (next >= m)
                {
                    int d = next / m;
                    int succ = d * m;
                    newCost = 1 + (1 - (double) succ / next) * cost[next - succ];
                }
                else
                {
                    newCost = 1 + cost[next];
                }
                if (newCost < cost[i])
                {
                    cost[i] = newCost;
                    move[i] = 0;
                }
            }
            for (int j = 1; j <= i / 2; j++)
            {
                double newCost = (double) j / i * cost[j] + (double) (i - j) / i * cost[i - j];
                if (newCost < cost[i])
                {
                    cost[i] = newCost;
                    move[i] = j;
                }
            }
        }
    }

    currCode = 0;
    for (int x = 1; x < m; x++)
    {
        codesStart[x] = -1;
    }

    encodeDfs(1);

    for (int code = 0; code < currCode; code++)
    {
        auto [curr, val] = codeToCurrVal[code];

        for (int r = 0; r < n; r++)
        {
            int next = curr * n;
            int nextVal = val * n + r;

            if (next >= m)
            {
                int d = next / m;
                int succ = d * m;
                if (nextVal < succ)
                {
                    codeToRet[code][r] = {true, nextVal % m};
                }
                else
                {
                    codeToRet[code][r] = {false, getCode(next - succ, nextVal % m)};
                }
            }
            else
            {
                codeToRet[code][r] = {false, getCode(next, nextVal)};
            }
        }
    }
}

std::pair<bool, int> proc(int x, int r)
{
    return codeToRet[x][r];
}
