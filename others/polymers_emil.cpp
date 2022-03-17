#include <iostream>
#include <algorithm>

typedef unsigned long long ull;

const int ALPHA_SIZE = 26;
const int MAX_TRANS = 128;
const ull MOD = 1e9 + 7;

int n, m;

ull cnts[ALPHA_SIZE * ALPHA_SIZE];
ull cnts2[ALPHA_SIZE * ALPHA_SIZE];
std::pair<int, int> trans[ALPHA_SIZE * ALPHA_SIZE];

ull ans[ALPHA_SIZE];

void input()
{
    std::string word;
    char c;

    std::cin >> word;
    for (int i = 0; i < (int) word.size() - 1; ++i)
    {
        ++cnts[(word[i] - 'A') * ALPHA_SIZE + word[i + 1] - 'A'];
    }

    ++ans[word[0] - 'A'];
    if ((int) word.size() > 1) ++ans[word[word.size() - 1] - 'A'];

    std::fill(trans, trans + ALPHA_SIZE * ALPHA_SIZE, std::make_pair(-1, -1));

    std::cin >> n >> m;
    for (int i = 0; i < m; ++i)
    {
        std::cin >> word >> c;
        trans[(word[0] - 'A') * ALPHA_SIZE + word[1] - 'A'] = {(word[0] - 'A') * ALPHA_SIZE + c - 'A', (c - 'A') * ALPHA_SIZE + word[1] - 'A'};
    }
}

void output()
{
    for (int i = 0; i < ALPHA_SIZE; ++i)
    {
        std::cout << (char) (i + 'A') << " " << ans[i] << std::endl;
    }
}

int numCodes;
int pairToCode[ALPHA_SIZE * ALPHA_SIZE];
int codeToPair[MAX_TRANS * 2];

void encodePairs()
{
    std::fill(pairToCode, pairToCode + ALPHA_SIZE * ALPHA_SIZE, -1);

    for (const auto& t : trans)
    {
        if (t.first == -1 || t.second == -1) continue;

        if (pairToCode[t.first] == -1)
        {
            pairToCode[t.first] = numCodes;
            codeToPair[numCodes++] = t.first;
        }

        if (pairToCode[t.second] == -1)
        {
            pairToCode[t.second] = numCodes;
            codeToPair[numCodes++] = t.second;
        }
    }
}

void manualIter()
{
    std::fill(cnts2, cnts2 + ALPHA_SIZE * ALPHA_SIZE, 0);

    for (int i = 0; i < ALPHA_SIZE * ALPHA_SIZE; ++i)
    {
        if (trans[i].first == -1 || trans[i].second == -1) cnts2[i] += cnts[i];
        else
        {
            cnts2[trans[i].first] += cnts[i];
            cnts2[trans[i].second] += cnts[i];
        }
    }

    std::copy(cnts2, cnts2 + ALPHA_SIZE * ALPHA_SIZE, cnts);
}

ull vector[MAX_TRANS * 2];
ull vector2[MAX_TRANS * 2];
ull matrix[MAX_TRANS * 2][MAX_TRANS * 2];

void constructVector()
{
    for (int i = 0; i < numCodes; ++i)
    {
        vector[i] = cnts[codeToPair[i]];
    }
}

void constructMatrix()
{
    for (int i = 0; i < numCodes; ++i)
    {
        int pair = codeToPair[i];

        if (trans[pair].first == -1 || trans[pair].second == -1) matrix[i][i] += 1;
        else
        {
            matrix[pairToCode[trans[pair].first]][i] += 1;
            matrix[pairToCode[trans[pair].second]][i] += 1;
        }
    }
}

ull res[MAX_TRANS * 2][MAX_TRANS * 2];

void matrixMultiply(ull left[MAX_TRANS * 2][MAX_TRANS * 2], const ull right[MAX_TRANS * 2][MAX_TRANS * 2])
{
    for (int i = 0; i < numCodes; ++i)
    {
        for (int j = 0; j < numCodes; ++j)
        {
            res[i][j] = 0;
        }
    }

    for (int i = 0; i < numCodes; ++i)
    {
        for (int k = 0; k < numCodes; ++k)
        {
            for (int j = 0; j < numCodes; ++j)
            {
                res[i][j] += left[i][k] * right[k][j] % MOD;
            }
        }
    }

    for (int i = 0; i < numCodes; ++i)
    {
        for (int j = 0; j < numCodes; ++j)
        {
            left[i][j] = res[i][j] % MOD;
        }
    }
}

ull accum[MAX_TRANS * 2][MAX_TRANS * 2];

void initAccum()
{
    for (int i = 0; i < numCodes; ++i)
    {
        accum[i][i] = 1;
    }
}

void matrixPow(int n)
{
    if (n == 0) return;

    if (n % 2 == 1) matrixMultiply(accum, matrix);
    matrixMultiply(matrix, matrix);
    matrixPow(n / 2);
}

void matrixVectorMultiply()
{
    for (int i = 0; i < numCodes; ++i)
    {
        for (int j = 0; j < numCodes; ++j)
        {
            vector2[i] += accum[i][j] * vector[j] % MOD;
        }
        vector2[i] %= MOD;
    }

    std::copy(vector2, vector2 + numCodes, vector);
}

void extractFromVector()
{
    for (int i = 0; i < numCodes; ++i)
    {
        cnts[codeToPair[i]] = vector[i];
    }
}

ull qpow(ull x, int n)
{
    if (n == 0) return 1;

    ull next = qpow(x, n / 2);
    next = next * next % MOD;

    if (n % 2 == 0) return next;
    else return next * x % MOD;
}

void findAns()
{
    for (int i = 0; i < ALPHA_SIZE * ALPHA_SIZE; ++i)
    {
        ans[i / ALPHA_SIZE] += cnts[i];
        ans[i % ALPHA_SIZE] += cnts[i];
    }

    for (int i = 0; i < ALPHA_SIZE; ++i)
    {
        ans[i] %= MOD;
        ans[i] *= qpow(2, MOD - 2);
        ans[i] %= MOD;
    }
}

void solve()
{
    encodePairs();

    if (n >= 1)
    {
        manualIter();
        constructVector();
        constructMatrix();
        initAccum();
        matrixPow(n - 1);
        matrixVectorMultiply();
        extractFromVector();
    }

    findAns();
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    input();
    solve();
    output();

    return 0;
}