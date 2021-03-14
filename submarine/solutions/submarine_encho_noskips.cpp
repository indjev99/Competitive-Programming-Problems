#include <iostream>
#include <stdio.h>
#include <vector>
using namespace std;

struct theory
{
    int x, v;

    theory(int x, int v): x(x), v(v) {}
};

int d, k;
bool isDead[201][201];

vector<theory> pos[201];

int modded(int x)
{
    if (x < 0)
        x += (abs(x) / d + 10) * d;

    return x %= d;
}

vector<int> shots;

int main()
{
    int i, j;

    scanf("%d %d", &d, &k);

    for (int iter=0;iter<d*k;iter++)
    {
        for (i=0;i<d;i++)
        {
            pos[i].clear();
        }

        for (i=0;i<d;i++)
        {
            for (j=-k;j<=k;j++)
            {
                if (j == 0)
                    continue;
                if (isDead[i][j+k])
                    continue;

                pos[ modded(i + j * iter) ].push_back(theory(i, j));
            }
        }

        int maxSize = pos[0].size();
        int id = 0;
        for (i=1;i<d;i++)
        {
            if (pos[i].size() > maxSize)
            {
                maxSize = pos[i].size();
                id = i;
            }
        }

        if (maxSize == 0)
            break;

        shots.push_back(id);
        for (i=0;i<pos[id].size();i++)
        {
            isDead[ pos[id][i].x ][ pos[id][i].v + k ] = true;
        }
    }

    fprintf(stderr, "Doing %d shots\n", (int)shots.size());

    printf("%d\n", (int)shots.size());

    for (i=0;i<shots.size();i++)
    {
        printf("%d %d\n", i, shots[i]);
    }

    return 0;
}
