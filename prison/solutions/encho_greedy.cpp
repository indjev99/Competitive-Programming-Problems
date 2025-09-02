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

int setup_explore(int m)
{
    int total = 1600;
    for (int i = 0; i < m; i++)
    {
        if (total <= 0)
            break;
        printf("\n\n\n");
        for (int j = i + 1; j < m; j++)
        {
            if (total <= 0)
                break;

            for (int in = j + 1; in < m; in++)
            {
                if (total <= 0)
                    break;

                if (OK(i, j, in))
                {
                    commit(i, j, in);
                    total--;
                    printf("%d %d %d\t%d\n", i, j, in, in - j);
                }
            }
        }
    }

    return 0;
}

int diffs[4501];
int dL;

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

        bool firstTime = true;
        dL = 0;
        int j = secondPivot;
        while(j < m)
        {
            for (int loop = 0; loop < secondLength; loop++)
            {
                /// i j ?

                int in = -1;
                if (firstTime)
                {
                    for (int iter = j + 1; iter < m; iter++)
                    {
                        if (OK(i, j, iter))
                        {
                            in = iter;
                            diffs[loop] = iter - j;
                            break;
                        }
                    }
                }
                else
                {
                    in = j + diffs[loop];
                    if (in != -1 && in < m)
                    {
                        assert(OK(i, j, in));
                    }
                }

                if (in != -1 && in < m)
                {
                    //printf("%d %d %d\n", i, j, in);
                    commit(i, j, in);
                }

                j++;

                if (in == -1 && in < m)
                    break;
            }
            j--;

            j += secondHops;
            firstTime = false;
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
