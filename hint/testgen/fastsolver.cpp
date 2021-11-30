#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

const int MAXN = 500111;

int F[2][MAXN];
int active;
void innerSolve(int *A, int n, int *B, int m)
{
    int i, j;

    for (i=0;i<=m+1;i++)
    {
        F[0][i] = 0;
        F[1][i] = 0;
    }

    for (i=1;i<=n;i++)
    {
        int op = active;
        active ^= 1;

        for (j=1;j<=m;j++)
        {
            if (A[i] == B[j])
                F[active][j] = F[op][j - 1] + 1;
            else
                F[active][j] = max(F[active][j - 1], F[op][j]);
        }
    }
}

int G[MAXN];
vector< pair<int, int> > solve(int *A, int n, int *B, int m)
{
    vector< pair<int, int> > answer;
    if (n == 0 || m == 0)
        return answer;

    if (n == 1)
    {
        for (int i = 1; i <= m; i++)
        {
            if (A[1] == B[i])
            {
                answer.push_back(make_pair(1, i));
                break;
            }
        }

        return answer;
    }

    int midRow = n / 2;

    /// Solve left part
    innerSolve(A, midRow, B, m);
    for (int i = 1; i <= m; i++)
    {
        G[i] = F[active][i];
    }

    /// Solve right part
    reverse(B + 1, B + 1 + m);
    reverse(A + midRow + 1, A + 1 + n);
    innerSolve(A + midRow, n - midRow, B, m);
    reverse(A + midRow + 1, A + 1 + n);
    reverse(B + 1, B + 1 + m);

    reverse(F[active] + 1, F[active] + 1 + m);

    int bestY = 0, bestScore = F[active][1];
    for (int i = 1; i <= m; i++)
    {
        if (G[i] + F[active][i + 1] > bestScore)
        {
            bestScore = G[i] + F[active][i + 1];
            bestY = i;
        }
    }

    answer = solve(A, midRow, B, bestY);
    vector< pair<int, int> > subResult = solve(A + midRow, n - midRow, B + bestY, m - bestY);
    for (int i = 0; i < (int) subResult.size(); i++)
    {
        answer.push_back(make_pair(subResult[i].first + midRow, subResult[i].second + bestY));
    }

    return answer;
}

int n, m;
int A[MAXN], B[MAXN];

int main()
{
    int i;
    mt19937 mt(1337);

    scanf("%d", &n);
    scanf("%d", &m);

    for (i=1;i<=n;i++)
    {
        scanf("%d", &A[i]);
    }

    for (i=1;i<=m;i++)
    {
        scanf("%d", &B[i]);
    }

    vector< pair<int, int> > res = solve(A, n, B, m);

    printf("%d\n", (int) res.size());

    for (i=0;i<(int)res.size();i++)
    {
        if (i > 0)
        {
            if (res[i].first <= res[i-1].first || res[i].second <= res[i-1].second)
            {
                printf("ERROR\n");
                return 0;
            }
        }

        if (A[ res[i].first ] != B[ res[i].second ])
        {
            printf("ERROR 2\n");
            return 0;
        }

        if (i != 0)
            printf(" ");
        printf("%d", A[ res[i].first ]);
    }
    printf("\n");

    return 0;
}
