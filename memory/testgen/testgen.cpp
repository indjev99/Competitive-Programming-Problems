#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>
#include <functional>

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

#define K_LARGE 0
#define K_UNIFORM 1
#define K_SQRT 2
#define K_SMALL 3
#define K_ONE 4

#define T_EQUAL 0
#define T_RECRAND 1
#define T_EXPDECAY 2
#define T_ONEBIG 3
#define T_BIGSMALL 4
#define T_ADDDECAY 5

int n = 1000;
int kType;
int type;

std::vector<int> parts;

void recRand(int left, int right, int n)
{
    if (left == right) return;
    if (left + 1 == right)
    {
        parts[left] = n;
        return;
    }

    int midN = randNum(0, n + 1);
    recRand(left, (left + right) / 2, midN);
    recRand((left + right) / 2, right, n - midN);
}

void genTest(std::ofstream& outfile)
{
    int k = 0;

    if (type == T_ADDDECAY && kType == K_LARGE) kType = K_UNIFORM;
    
    if (kType == K_LARGE) k = randNum(n / 4, n);
    else if (kType == K_UNIFORM) k = randNum(2, n / 5);
    else if (kType == K_SQRT) k = randNum(2, sqrt(n + 1));
    else if (kType == K_SMALL) k = randNum(2, 6);
    else if (kType == K_ONE) k = 1;

    if (type == T_BIGSMALL && k == 2) type = T_ONEBIG;

    parts.resize(k);
    std::fill(parts.begin(), parts.end(), 0);

    if (type == T_EQUAL)
    {
        for (int i = 0; i < k; ++i)
        {
            parts[i] = (i + 1) * n / k - i * n / k;
        }
    }
    else if (type == T_RECRAND)
    {
        recRand(0, k, n);
    }
    else if (type == T_EXPDECAY)
    {
        double decay = randNum(100, 901) * 1.0 / 1000;
        double total = 1 / (1 - decay) - pow(decay, k) / (1 - decay);
        double scale = n / total;
        int sum = 0;
        for (int i = 0; i < k; ++i)
        {
            parts[i] = round(scale * pow(decay, i));
            sum += parts[i];
        }
        while (sum < n)
        {
            int i = randNum(0, k);
            ++parts[i];
            ++sum;
        }
        while (sum > n)
        {
            int i = randNum(0, k);
            if (parts[i] == 0) continue;
            --parts[i];
            --sum;
        }
    }
    else if (type == T_ONEBIG)
    {
        int big = randNum(n * 2 / k - 1, n);
        parts[0] = big;
        for (int i = 0; i < k - 1; ++i)
        {
            parts[i + 1] = (i + 1) * (n - big) / (k - 1) - i * (n - big) / (k - 1);
        }
    }
    else if (type == T_BIGSMALL)
    {
        int numBig = randNum(2, k);
        int big = randNum(n / k, n / numBig + 1);
        assert(numBig * big < n);
        parts[0] = big;
        for (int i = 0; i < numBig; ++i)
        {
            parts[i] = big;
        }
        for (int i = 0; i < k - numBig; ++i)
        {
            parts[i + numBig] = (i + 1) * (n - numBig * big) / (k - numBig) - i * (n - numBig * big) / (k - numBig);
        }
    }
    else if (type == T_ADDDECAY)
    {
        repeatAddDecay:
        double decay = randNum(n * 250 / k / k, n * 1250 / k / k) * 1.0 / 1000;
        std::cerr << n << " " << k << " : " << decay << std::endl;
        double total = k * (-decay * (k - 1)) / 2;
        double add = (n - total) / k;
        if (add - decay * (k - 1) < 0) goto repeatAddDecay;
        int sum = 0;
        for (int i = 0; i < k; ++i)
        {
            parts[i] = round(add - decay * i);
            sum += parts[i];
        }
        while (sum < n)
        {
            int i = randNum(0, k);
            ++parts[i];
            ++sum;
        }
        while (sum > n)
        {
            int i = randNum(0, k);
            if (parts[i] == 0) continue;
            --parts[i];
            --sum;
        }
    }

    std::sort(parts.begin(), parts.end(), std::greater<int>());

    outfile << k << ' ' << randNum(0, 1000000) << '\n';
    for (int i = 0; i < k; ++i)
    {
        if (i > 0) outfile << ' ';
        outfile << parts[i];
    }
    outfile << "\n";
    outfile << std::flush;

    int sum = 0;
    for (int elem : parts)
    {
        sum += elem;
        assert(elem >= 0);
    }
    assert(sum == n);
}

int main()
{
    std::ios::sync_with_stdio(false);

    generator.seed(42);

    int num;
    bool subtask;
    while (true)
    {
        num = -1;
        std::cout << "Test num? ";
        std::cin >> num;
        if (num < 0) return 0;
        std::cout << "subtask? ";
        std::cin >> subtask;
        for (int i = 0; i < (subtask ? 13 : 40); ++i)
        {
            std::string nums = "";
            nums += (num / 10 + '0');
            nums += (num % 10 + '0');
            std::string name = "memory." + nums + ".in";
            std::ofstream outfile(name);

            if (subtask)
            {
                type = T_EQUAL;
                if (i == 12) kType = K_ONE;
                else kType = i % 4;
            }
            else
            {
                kType = i % 4;
                type = 1 + (i % 20) / 4;
            }

            std::cerr << name << " " << kType << " " << type << std::endl;

            genTest(outfile);
            outfile.close();

            ++num;
        }
    }
    return 0;
}
