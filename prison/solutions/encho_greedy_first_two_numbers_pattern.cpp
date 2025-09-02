#include <vector>
#include <stdio.h>
#include <assert.h>
#include "prison.h"
using namespace std;

bool seen[4501][4501];
int answer[4501][4501];
pair<int, pair<int, int>> encoding[4000111];
int ctr = 0;

bool OK(int x, int y, int z)
{
    return !seen[x][y] && !seen[x][z] && !seen[y][z];
}

void commit(int x, int y, int z)
{
    encoding[ctr] = {x, {y, z}};

    answer[x][y] = ctr;
    answer[x][z] = ctr;
    answer[y][z] = ctr;

    answer[y][x] = ctr;
    answer[z][x] = ctr;
    answer[y][z] = ctr;

    seen[x][y] = true;
    seen[x][z] = true;
    seen[y][z] = true;

    seen[y][x] = true;
    seen[z][x] = true;
    seen[z][y] = true;

    ctr++;
}

int setup(int m)
{
    int secondPivot = 1;
    int secondLength = 1;
    int secondHops = 2;
    for (int i = 0; i < m; i++)
    {
        if (i == secondPivot)
        {
            secondPivot = secondPivot * 2 + 1;
            secondLength *= 2;
            secondHops = secondHops * 2 - 1;
        }

        int j = secondPivot;
        while(j < m)
        {
            for (int loop = 0; loop < secondLength; loop++)
            {
                int in = -1;
                for (int iter = j + 1; iter < m; iter++)
                {
                    if (OK(i, j, iter))
                    {
                        in = iter;
                        break;
                    }
                }

                if (in != -1 && in < m)
                {
                    commit(i, j, in);
                }

                j++;

                if (in == -1 && in < m)
                    break;
            }
            j--;

            j += secondHops;
        }
    }

    return ctr;
}

vector<int> encode(int a)
{
    return {encoding[a].first, encoding[a].second.first, encoding[a].second.second};
}

int decode(int x, int y)
{
    if (x > y)
        swap(x, y);
    return answer[x][y];
}
