#include <iostream>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <queue>
#include <set>
#include <random>
#include <cassert>

std::mt19937 generator;

long long randInt(long long lb, long long ub)
{
    assert(ub > lb);
    std::uniform_int_distribution<long long> distribution(lb, ub - 1);
    return distribution(generator);
}

long long randInt(long long ub)
{
    return randInt(0, ub);
}

double randReal(double lb = 0, double ub = 1)
{
    std::uniform_real_distribution<double> distribution(lb, ub);
    return distribution(generator);
}

bool oneIn(long long chances)
{
    if (chances == 0) return false;
    return randInt(chances) == 0;
}

struct TestParams
{
    int n;
    int type;
    int flip;
};

constexpr int MAX_N = 4e6;

int sums[MAX_N + 1];

void fillSumsRec(int l, int r)
{
    if (l + 1 >= r) return;

    int m = randInt(l + 1, r);

    int max = std::min(sums[l] + m - l, sums[r] + r - m);
    int min = std::max(sums[l] - m + l, sums[r] - r + m);
    min = std::max(min, max % 2 == 0 ? 0 : 1);

    int val = min + randInt(0, (max - min) / 2 + 1) * 2;

    sums[m] = val;

    fillSumsRec(l, m);
    fillSumsRec(m, r);
}

int recVals[MAX_N + 1];

void loglogRec(int l, int r)
{
    int n = r - l;
    assert(n % 2 == 0);

    if (n <= 14)
    {
        switch (n)
        {
        case 0:
            return;
        case 2:
            recVals[l++] = -1;
            recVals[l++] = 1;
            return;
        case 4:
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            return;
        case 6:
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            return;
        case 8:
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            return;
        case 10:
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            return;
        case 12:
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            return;
        case 14:
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            recVals[l++] = 1;
            recVals[l++] = -1;
            recVals[l++] = 1;
            return;
        }
    }

    int root = std::sqrt(n);
    if (root % 2 == 1) --root;

    int i = l;
    int sum = 0;

    for (int j = 0; j < root + (n - 2 * root) / (root - 1); ++j)
    {
        recVals[i++] = -1;
        sum -= 1;
    }

    for (int j = 0; j < root; ++j)
    {
        recVals[i++] = 1;
        sum += 1;
    }

    assert(i <= r);

    while (r - (i + root) >= -sum)
    {
        loglogRec(i, i + root - 2);
        i = i + root - 2;
        recVals[i++] = 1;
        sum += 1;
    }

    while (i < r)
    {
        if (sum == 0)
        {
            recVals[i++] = -1;
            sum -= 1;
        }
        else
        {
            recVals[i++] = 1;
            sum += 1;
        }
    }

    assert(sum == 0);
}

void loglog(int n)
{
    int root = sqrt(n) + 1;
    for (int i = 0; i < 2 * root; ++i)
    {
        recVals[i] = 1;
    }
    for (int i = n - 2 * root; i < n; ++i)
    {
        recVals[i] = -1;
    }
    loglogRec(2 * root, n - 2 * root);
}

void generate(TestParams test, std::ofstream& infile)
{
    int n = test.n;
    int type = test.type;
    int flip = test.flip;

    std::vector<int> vals(n, 0);

    if (type == 0)
    {
        int sum = 0;
        for (int i = 0; i < n; ++i)
        {
            if (sum == 0)
            {
                vals[i] = 1;
                sum += 1;
                continue;
            }

            if (sum == n - i)
            {
                vals[i] = -1;
                sum -= 1;
                continue;
            }

            int leftUp = (n - i - sum) / 2;
            int leftDown = (n - i + sum) / 2;

            bool up = randInt(leftUp + leftDown) < leftUp;

            vals[i] = up ? 1 : -1;
            sum += vals[i];
        }
    }

    else if (type == 1)
    {
        int sum = 0;
        for (int i = 0; i < n; ++i)
        {
            if (sum == 0)
            {
                vals[i] = 1;
                sum += 1;
                continue;
            }

            if (sum == n - i)
            {
                vals[i] = -1;
                sum -= 1;
                continue;
            }

            int leftUp = (n - i - sum) / 2;
            int leftDown = (n - i + sum) / 2;

            if (i < n / 2)
            {
                leftUp = 19;
                leftDown = 1;
            }

            bool up = randInt(leftUp + leftDown) < leftUp;

            vals[i] = up ? 1 : -1;
            sum += vals[i];
        }
    }

    else if (type == 2)
    {
        int sum = 0;
        for (int i = 0; i < n; ++i)
        {
            if (sum == 0)
            {
                vals[i] = 1;
                sum += 1;
                continue;
            }

            if (sum == n - i)
            {
                vals[i] = -1;
                sum -= 1;
                continue;
            }

            int leftUp = (n - i - sum) / 2;
            int leftDown = (n - i + sum) / 2;

            if (i < n / 4)
            {
                leftUp = 19;
                leftDown = 1;
            }
            else if (i < n / 2)
            {
                leftDown = 19;
                leftUp = 1;
            }
            else if (i < 3 * n / 4)
            {
                leftUp = 19;
                leftDown = 1;
            }

            bool up = randInt(leftUp + leftDown) < leftUp;

            vals[i] = up ? 1 : -1;
            sum += vals[i];
        }
    }

    else if (type == 3)
    {
        int sum = 0;
        for (int i = 0; i < n; ++i)
        {
            if (sum == 0)
            {
                vals[i] = 1;
                sum += 1;
                continue;
            }

            if (sum == n - i)
            {
                vals[i] = -1;
                sum -= 1;
                continue;
            }

            if (i < n / 20 + 10)
            {
                vals[i] = 1;
            }
            else if ((i - (n / 20 + 10)) % 19 < 10)
            {
                vals[i] = -1;
            }
            else
            {
                vals[i] = 1;
            }

            sum += vals[i];
        }
    }

    else if (type == 4)
    {
        int origChunk = n / 3;
        int chunk = origChunk;

        int i = 0;
        int sum = 0;

        for (; i < chunk; ++i)
        {
            vals[i] = 1;
            sum += vals[i];
        }

        chunk = chunk * 2 / 3 - 1;

        while (chunk >= 3)
        {
            int j = 0;
            for (; j < chunk / 2 + 1; ++j)
            {
                vals[i] = -1;
                sum += vals[i];
                ++i;
            }

            for (; j < chunk; ++j)
            {
                vals[i] = 1;
                sum += vals[i];
                ++i;
            }

            chunk = chunk / 3 - 1;
        }

        while (sum <= origChunk)
        {
            vals[i] = 1;
            sum += vals[i];
            ++i;
        }

        for (; i < n; ++i)
        {
            if (sum == 0)
            {
                vals[i] = 1;
                sum += 1;
                continue;
            }

            vals[i] = -1;
            sum -= 1;
            continue;
        }
    }

    else if (type == 5)
    {
        int i = 0;
        int sum = 0;

        for (int j = 1; ; ++j)
        {
            int max = 1;
            int j2 = j;
            while (j2 % 2 == 0)
            {
                j2 /= 2;
                max += 1;
            }

            if (i + 2 * max > n) break;

            for (int t = 0; t < max; ++t)
            {
                vals[i] = 1;
                sum += 1;
                ++i;
            }

            for (int t = 0; t < max; ++t)
            {
                vals[i] = -1;
                sum -= 1;
                ++i;
            }
        }

        while (i < n)
        {
            if (sum == 0)
            {
                vals[i] = 1;
                sum += 1;
                i += 1;
                continue;
            }

            vals[i] = -1;
            sum -= 1;
            i += 1;
            continue;
        }
    }

    else if (type == 6)
    {
        sums[0] = 0;
        sums[n] = 0;
        fillSumsRec(0, n);
        for (int i = 0; i < n; ++i)
        {
            vals[i] = sums[i + 1] - sums[i];
        }
    }

    else if (type == 7)
    {
        loglog(n);
        std::copy(recVals, recVals + n, vals.begin());
    }

    if (flip)
    {
        std::reverse(vals.begin(), vals.end());
        for (int i = 0; i < n; ++i)
        {
            vals[i] = - vals[i];
        }
    }

    int sum = 0;
    for (int i = 0; i < n; ++i)
    {
        assert(vals[i] == 1 || vals[i] == -1);
        sum += vals[i];
        assert(sum >= 0);

        // if (i > 0) std::cerr << " ";
        // std::cerr << sum;
    }
    assert(sum == 0);

    // std::cerr << std::endl;

    infile << n << "\n";
    for (int i = 0; i < n; ++i)
    {
        infile << (vals[i] == 1 ? "(" : ")");
    }
    infile << "\n";
}

std::vector<int> ns = {
    500,
    3000,
    15000,
    300000,
    1500000,
    4000000,
};

int main()
{
    generator.seed(1337);

    int num = 1;
    for (int n : ns)
    {
        for (int type = 0; type < 8; ++type)
        {
            for (int flip = 0; flip < 2; ++flip)
            {
                TestParams test = {n, type, flip};

                std::string nums = "";
                nums += num / 10 + '0';
                nums += num % 10 + '0';

                std::string inName = "brackets." + nums + ".in";

                std::cerr << "Test " << nums << ": " << test.n << " " << test.type << " " << test.flip << std::endl;

                std::ofstream infile(inName);

                generate(test, infile);

                ++num;
            }
        }
    }

    return 0;
}
