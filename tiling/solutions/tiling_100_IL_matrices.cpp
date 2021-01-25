#include <iostream>

typedef unsigned long long ull;

const ull MOD = 1e9 + 7;

long long n;
bool Is, Ls;

ull ans;

void input()
{
    int t;
    std::cin >> n >> t;
    Is = t & 1;
    Ls = t & 2;
}

void output()
{
    std::cout << ans << std::endl;
}

const int DIM = 4;

struct Vector
{
    ull x[DIM]; // curr_base, curr_ridge, prev_base, prev_ridge
};

struct Matrix
{
    ull a[DIM][DIM];
};

Matrix operator*(const Matrix& left, const Matrix& right)
{
    Matrix res;
    for (int i = 0; i < DIM; ++i)
    {
        for (int j = 0; j < DIM; ++j)
        {
            res.a[i][j] = 0;
            for (int k = 0; k < DIM; ++k)
            {
                res.a[i][j] += left.a[i][k] * right.a[k][j];
            }
            res.a[i][j] %= MOD;
        }
    }
    return res;
}

Matrix operator+(const Matrix& left, const Matrix& right)
{
    Matrix res;
    for (int i = 0; i < DIM; ++i)
    {
        for (int j = 0; j < DIM; ++j)
        {
            res.a[i][j] = (left.a[i][j] + right.a[i][j]) % MOD;
        }
    }
    return res;
}

Vector operator*(const Matrix& left, const Vector& right)
{
    Vector res;
    for (int i = 0; i < DIM; ++i)
    {
        res.x[i] = 0;
        for (int k = 0; k < DIM; ++k)
        {
            res.x[i] += left.a[i][k] * right.x[k];
        }
        res.x[i] %= MOD;
    }
    return res;
}

const Matrix identity_matrix = {{
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
}};

const Matrix default_matrix = {{
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0}
}};

const Matrix I_matrix = {{
    {1, 0, 1, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
}};

const Matrix L_matrix = {{
    {0, 0, 0, 1},
    {2, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
}};

Matrix qpow(const Matrix& left, long long power)
{
    if (power == 0) return identity_matrix;

    Matrix other = qpow(left * left, power / 2);
    
    if (power % 2 == 0) return other;
    else return other * left;
}

void solve()
{
    Vector init = {{1, 0, 0, 0}};

    Matrix transition = default_matrix;
    if (Is) transition = transition + I_matrix;
    if (Ls) transition = transition + L_matrix;

    Vector curr = qpow(transition, n) * init;

    ans = curr.x[0];
}

int main()
{
    input();
    solve();
    output();

    return 0;
}