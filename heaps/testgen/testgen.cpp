#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <assert.h>

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

struct CustomHeap
{
    int big;
    int small;

    CustomHeap(int big, int small):
        big(big),
        small(small) {}
};

struct NimHeap
{
    int big;
    int small;

    NimHeap(int big, int small):
        big(big),
        small(small) {}
};

const int MAX_B = 3000;

int n, k;

bool ans;

bool operator!=(const NimHeap& left, const NimHeap& right)
{
    return left.big != right.big || left.small != right.small;
}

bool operator<(const NimHeap& left, const NimHeap& right)
{
    return left.big < right.big || (left.big == right.big && left.small < right.small);
}

NimHeap operator^=(NimHeap& left, const NimHeap& right)
{
    left.big ^= right.big;
    left.small ^= right.small;
    return left;
}

bool isWin(const NimHeap& nimHeap)
{
    return nimHeap != NimHeap(0, 0);
}

bool foundNimEquivs[MAX_B + 1];
std::vector<NimHeap> nimEquivs[MAX_B + 1];

NimHeap getNimEquiv(const CustomHeap& heap);
void findNimEquivs(int heapBig)
{
    foundNimEquivs[heapBig] = true;

    for (int big = 0; big < heapBig; ++big)
    {
        int minSmall = (heapBig - big) * k;
        nimEquivs[heapBig].push_back(getNimEquiv(CustomHeap(big, minSmall - k)));
    }

    std::sort(nimEquivs[heapBig].begin(), nimEquivs[heapBig].end());
}

NimHeap getNimEquiv(const CustomHeap& heap)
{
    if (heap.small >= heap.big * k)
    {
        return NimHeap(heap.big, heap.small - heap.big * k);
    }

    if (!foundNimEquivs[heap.big]) findNimEquivs(heap.big);
    NimHeap baseAns = nimEquivs[heap.big][heap.small / k];
    return NimHeap(baseAns.big, baseAns.small + heap.small % k);
}

const int MAX_S = 1e7;

int MAX_KB;
int SUBTASK;

CustomHeap randCustomHeap()
{
    int big = randNum(0, MAX_KB + 1);
    int small;
    if (big == 1 && SUBTASK == 2) small = 0;
    else if (big == 0 || k == 0 || oneIn(2)) small = randNum(0, MAX_S + 1);
    else small = randNum(0, k * big);
    return CustomHeap(big, small);
}

#define T_RAND 0
#define T_ZERO_BOTH 1
#define T_ZERO_BIG 2
#define T_ZERO_SMALL 3

bool genTest(std::ofstream& outfile, int outcome)
{
    NimHeap total(0, 0);

    std::vector<CustomHeap> heaps;

    for (int i = 0; i < n - 3; ++i)
    {
        heaps.push_back(randCustomHeap());
        total ^= getNimEquiv(heaps.back());
    }

    if (SUBTASK == 2)
    {
        heaps.push_back(CustomHeap(1, 0));
        total ^= getNimEquiv(heaps.back());
    }
    else
    {
        heaps.push_back(randCustomHeap());
        total ^= getNimEquiv(heaps.back());
    }

    if (outcome == T_RAND)
    {
        heaps.push_back(randCustomHeap());
        total ^= getNimEquiv(heaps.back());

        if (oneIn(2))
        {
            heaps.push_back(randCustomHeap());
            total ^= getNimEquiv(heaps.back());
        }
        else
        {
            if ((total.big ^ total.small) > MAX_S) return false;
            heaps.push_back(CustomHeap(0, total.big ^ total.small));
            total ^= getNimEquiv(heaps.back());
        }
    }
    else
    {
        if (outcome == T_ZERO_BOTH)
        {
            if (total.small > MAX_S || total.big > MAX_KB || total.big * k > MAX_S) return false;
            heaps.push_back({0, total.small});
            total ^= getNimEquiv(heaps.back());
            heaps.push_back({total.big, total.big * k});
            total ^= getNimEquiv(heaps.back());
        }
        else if (outcome == T_ZERO_BIG)
        {
            if (oneIn(2))
            {
                heaps.push_back(randCustomHeap());
                total ^= getNimEquiv(heaps.back());
            }
            else
            {
                if ((1 ^ total.small) > MAX_S) return false;
                heaps.push_back(CustomHeap(0, 1 ^ total.small));
                total ^= getNimEquiv(heaps.back());
            }
            if (total.big > MAX_KB || total.big * k > MAX_S) return false;
            heaps.push_back({total.big, total.big * k});
            total ^= getNimEquiv(heaps.back());
        }
        else if (outcome == T_ZERO_SMALL)
        {
            heaps.push_back(randCustomHeap());
            total ^= getNimEquiv(heaps.back());
            if (total.small > MAX_S) return false;
            heaps.push_back({0, total.small});
            total ^= getNimEquiv(heaps.back());
        }
    }

    std::cerr << " " << (isWin(total) ? "Win" : "Loss") << " " << outcome << " " << total.big << " " << total.small << std::endl;

    std::shuffle(heaps.begin(), heaps.end(), generator);

    assert(n == (int) heaps.size());

    outfile << n << "\n";

    for (const CustomHeap& heap : heaps)
    {
        assert(heap.small >= 0);
        assert(heap.small <= MAX_S);
        assert(heap.big >= 0);
        assert(heap.big <= MAX_KB);
        outfile << heap.big << " " << heap.small << "\n";
    }

    return true;
}

void metaGenTest(std::ofstream& outfile, bool max)
{
    n = 1e4;
    int q = 10;
    k = randNum(MAX_KB / 2, MAX_KB + 1);

    if (max) k = MAX_KB;

    if (SUBTASK <= 3) k = 0;
    if (SUBTASK == 4) k = 1;

    outfile << k << " " << q << "\n";

    for (int i = 0; i <= MAX_B; ++i)
    {
        foundNimEquivs[i] = false;
        nimEquivs[i].clear();
    }


    for (int i = 0; i < q; ++i)
    {
        int outcome;
        if (oneIn(2)) outcome = T_ZERO_BOTH;
        else if (oneIn(3)) outcome = T_RAND;
        else if (oneIn(2)) outcome = T_ZERO_BIG;
        else outcome = T_ZERO_SMALL;
        while (!genTest(outfile, outcome)) {}
    }
}

int main()
{
    std::ios::sync_with_stdio(false);

    generator.seed(1337);

    int num;
    while (true)
    {
        std::cout << "Test num? ";
        std::cin >> num;
        if (num < 0) return 0;
        std::cout << "max_kb subtask? ";
        std::cin >> MAX_KB >> SUBTASK;
        for (int i = 0; i < (SUBTASK >= 5 ? 10 : 5); ++i)
        {
            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');

            std::cerr << "heaps." << nums << ".in" << std::endl;

            std::ofstream outfile("heaps." + nums + ".in");
            metaGenTest(outfile, i % 4 == 2);
            outfile.close();

            ++num;
        }
    }

    return 0;
}
