#include <iostream>
#include <iomanip>

const int MAX_N = 1e2;

double prob;
int n, l, r;
double ans;

double cost[MAX_N + 1]; // left right

double matrix[(MAX_N + 1) * (MAX_N + 1)][(MAX_N + 1) * (MAX_N + 1) + 1];

int state(int left, int right)
{
    return left * (n + 1) + right;
}

void solve()
{
    int states = (n + 1) * (n + 1);

    for (int i = 0; i < states; ++i)
    {
        for (int j = 0; j < states + 1; ++j)
        {
            matrix[i][j] = 0;
        }
    }

    for (int i = 0; i < states; ++i)
    {
        int left = i / (n + 1);
        int right = i % (n + 1);

        matrix[i][i] = 1;

        if (left == n && right == n)
        {
            matrix[i][states] = 0;
        }
        else if (left == right)
        {
            matrix[i][states] = 1;
            matrix[i][state(left + 1, right)] = -prob;
            matrix[i][state(left, right + 1)] = -(1 - prob);
        }
        else if (left < right)
        { 
            matrix[i][states] = 2;
            matrix[i][state(left + 1, right)] = -prob;

            double remProb = (1 - prob);
            for (int j = 0; j < left; ++j)
            {
                matrix[i][state(left - j, right)] -= (1 - prob) * remProb;
                remProb *= prob;
            }

            matrix[i][state(0, right)] -= remProb;
        }
        else if (right < left)
        { 
            matrix[i][states] = 2;
            matrix[i][state(left, right + 1)] = -(1 - prob);

            double remProb = prob;
            for (int j = 0; j < right; ++j)
            {
                matrix[i][state(left, right - j)] -= prob * remProb;
                remProb *= (1 - prob);
            }

            matrix[i][state(left, 0)] -= remProb;
        }
    }

    for (int i = 0; i < states; ++i)
    {
        int pivot = i;
        double pivotVal = matrix[i][i];

        for (int j = i + 1; j < states; ++j)
        {
            if (std::abs(matrix[j][i]) > std::abs(pivotVal))
            {
                pivotVal = matrix[j][i];
                pivot = j;
            }
        }

        for (int k = i; k < states + 1; ++k)
        {
            std::swap(matrix[i][k], matrix[pivot][k]);
            matrix[i][k] /= pivotVal;
        }

        for (int j = 0; j < states; ++j)
        {
            if (j == i) continue;

            double ratio = matrix[j][i];

            for (int k = i; k < states + 1; ++k)
            {
                matrix[j][k] -= matrix[i][k] * ratio;
            }
        }
    }

    ans = matrix[state(l, r)][states];
}

int main()
{
    std::cin >> prob >> n >> l >> r;
    solve();
    std::cout << std::setprecision(15) << ans << std::endl;

    return 0;
}
