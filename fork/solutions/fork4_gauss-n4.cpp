#include <iostream>
#include <iomanip>

const int MAX_N = 1e2;

double prob;
int n, l, r;
double ans;

double cost[MAX_N + 1]; // left right

double matrix[MAX_N + 1][MAX_N + 2];

double solveInner(int max, double prob, int target)
{
    for (int i = 0; i <= max; ++i)
    {
        for (int j = 0; j <= max + 1; ++j)
        {
            matrix[i][j] = 0;
        }
    }

    matrix[max][max] = 1;
    matrix[max][max + 1] = cost[max];

    for (int i = 0; i < max; ++i)
    {
        matrix[i][i] = 1;
        matrix[i][max + 1] = 2;
        matrix[i][i + 1] = -prob;

        double remProb = (1 - prob);
        for (int j = 0; j < i; ++j)
        {
            matrix[i][i - j] -= (1 - prob) * remProb;
            remProb *= prob;
        }

        matrix[i][0] -= remProb;
    }

    for (int i = 0; i <= max; ++i)
    {
        int pivot = i;
        double pivotVal = matrix[i][i];

        for (int j = i + 1; j <= max; ++j)
        {
            if (std::abs(matrix[j][i]) > std::abs(pivotVal))
            {
                pivotVal = matrix[j][i];
                pivot = j;
            }
        }

        for (int k = i; k <= max + 1; ++k)
        {
            std::swap(matrix[i][k], matrix[pivot][k]);
            matrix[i][k] /= pivotVal;
        }

        for (int j = 0; j <= max; ++j)
        {
            if (j == i) continue;

            double ratio = matrix[j][i];

            for (int k = i; k <= max + 1; ++k)
            {
                matrix[j][k] -= matrix[i][k] * ratio;
            }
        }
    }

    return matrix[target][max + 1];
}

void solve()
{
    if (l < r)
    {
        std::swap(l, r);
        prob = 1 - prob;
    }

    cost[n] = 0;

    for (int curr = n - 1; curr >= 0; --curr)
    {
        double costLeft = solveInner(curr + 1, 1 - prob, curr);
        double costRight = solveInner(curr + 1, prob, curr);
        cost[curr] = 1 + prob * costLeft + (1 - prob) * costRight;
    }

    ans = solveInner(l, 1 - prob, r);
}

int main()
{
    std::cin >> prob >> n >> l >> r;
    solve();
    std::cout << std::setprecision(15) << ans << std::endl;

    return 0;
}
