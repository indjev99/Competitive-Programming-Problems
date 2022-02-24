#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

const int MAX_N = 1e6;

std::mt19937 generator;
int randNum(int lb, int ub)
{
    std::uniform_int_distribution<int> distribution(lb, ub - 1);
    return distribution(generator);
}
bool oneIn(int chances)
{
    std::uniform_int_distribution<int> distribution(0, chances - 1);
    return distribution(generator) == 0;
}

int p[MAX_N];
int q[MAX_N];
int occurs[MAX_N];

bool isValid(int n)
{
    std::fill(occurs, occurs + n, 0);
    std::fill(p, p + n, 0);

	for (int i = 0; i < n; ++i)
	{
		int& curr = occurs[q[i] - 1];
		if (curr == 1 && (i < 2 || q[i] != q[i - 2])) return false;
		if (curr == 2) return false;
		++curr;
	}

    for (int i = 1; i < n && (i < 2 || q[i - 1] != p[i - 2]); i += 2)
    {
        p[i] = q[i - 1];
    }

	if (p[n - 2] != 0 && p[n - 2] != q[n - 1]) return false;

    return true;
}

#define M_NORMAL 1
#define M_VALID 2
#define M_PERM 3

int metaType;

#define T_RANDOM_VALID 0
#define T_VALID_FORCED 1
#define T_LONG_FORCED 2
#define T_LONG_GAP 3
#define T_RAND_PERM 4
#define T_INVALID_3 5
#define T_INVALID_DIST 6
#define T_INVALID_FORCED 7

int nMax;
int type;

bool genTest(std::ofstream& outfile)
{
    bool odd = oneIn(2);
    int n = nMax - odd;

    std::iota(p, p + n, 1);
    std::shuffle(p, p + n, generator);

    q[0] = p[1];
    q[n - 1] = p[n - 2];
    for (int i = 1; i < n - 1; ++i)
    {
        if (oneIn(2)) q[i] = p[i + 1];
        else q[i] = p[i - 1];
    }

    if (type == T_INVALID_3)
    {
        int pos = randNum(0, n - 4);

        if (q[pos] != q[pos + 2]) return false;
        
        q[pos + 4] = q[pos];
    }
    else if (type == T_INVALID_DIST)
    {
        int pos = randNum(2, n - 2);

        if (q[pos] == q[pos - 2] || q[pos] == q[pos + 2]) return false;

        if (oneIn(2)) q[pos + (oneIn(2) ? 1 : -1)] = q[pos];
        else
        {
            int other = randNum(0, n);
            if (abs(pos - other) <= 2) return false;

            q[other] = q[pos];
        }
    }
    else if (type == T_RAND_PERM)
    {
        std::copy(p, p + n, q);
    }
    else if (type == T_INVALID_FORCED || type == T_VALID_FORCED)
    {
        if (n % 2 == 0) return false;

        int pos = randNum(0, n / 2) * 2;

        for (int i = 0; i < n; i += 2)
        {
            if (i <= pos) q[i] = p[i + 1];
            else q[i] = p[i - 1];
        }

        int unseen = 0;

        for (int i = 0; i < n; ++i)
        {
            if ((i == 0 || q[i - 1] != p[i]) && (i == n - 1 || q[i + 1] != p[i]))
            {
                unseen = p[i];
                break;
            }
        }

        if (type == T_INVALID_FORCED && unseen == 0) return false;

        if (type == T_INVALID_FORCED) q[pos] = unseen;
    }
    else if (type == T_LONG_FORCED)
    {
        int pos1 = randNum(0, n);
        int pos2 = randNum(0, n);

        if (pos1 >= pos2) return false;

        for (int i = 0; i < pos1; i += 2)
        {
            q[i] = p[i + 1];
        }

        for (int i = n - 1; i >= pos2; i -= 2)
        {
            q[i] = p[i - 1];
        }
    }
    else if (type == T_LONG_GAP)
    {
        int pos1 = randNum(0, n);
        int pos2 = randNum(0, n);

        if (pos1 >= pos2) return false;

        for (int i = pos1; i < pos2; ++i)
        {
            q[i] = p[i + 1];
        }
    }

    bool shouldBeValid = type < 4 || (type == 4 && metaType == M_VALID);
    bool shouldBeInvalid = type > 4;

    bool valid = isValid(n);
    
    if ((shouldBeValid && !valid) || (shouldBeInvalid && valid)) return false;

    outfile << n << "\n";

    for (int i = 0; i < n; ++i)
    {
        if (i > 0) outfile << " ";
        outfile << q[i];
    }

    outfile << "\n";

    return true;
}

int main()
{
    std::ios::sync_with_stdio(false);

    int num;

    while (true)
    {
        std::cout << "Test num? ";
        std::cin >> num;
        if (num < 0) return 0;
        std::cout << "n meta? ";
        std::cin >> nMax >> metaType;
        for (int i = 0; i < 8; ++i)
        {
            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
            std::ofstream outfile("permutation." + nums + ".in");

            type = i % 8;

            if (metaType == M_PERM) type = T_RAND_PERM;
            else if (metaType == M_VALID) type = i % 5;

            bool succ = genTest(outfile);
            outfile.close();

            if (!succ)
            {
                --i;
                continue;
            }

            ++num;
        }
    }
    return 0;
}
