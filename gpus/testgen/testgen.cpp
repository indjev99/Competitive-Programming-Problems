#include <iostream>
#include <fstream>
#include <algorithm>
#include <tuple>
#include <vector>
#include <queue>
#include <set>
#include <random>
#include <cassert>

int n, m;

std::vector<int> gpuCosts;
std::vector<int> reqTimes;

__int128 ans;

std::vector<int> reqTimeCounts;

int maxTime;
int maxGpus;
std::vector<int> usedGpuCounts;
std::vector<int> donePerGpus;
std::vector<long long> sPerGpus;

void initBuild()
{
    long long s = 0;

    int f = maxTime + 1;

    usedGpuCounts.clear();
    donePerGpus.clear();
    sPerGpus.clear();
    usedGpuCounts.resize(m, 0);
    donePerGpus.reserve(m + 1);
    sPerGpus.reserve(m + 1);

    maxGpus = 0;
    donePerGpus.push_back(0);
    sPerGpus.push_back(0);

    std::vector<int> doneTimeCounts(f, 0);

    int done = 0;

    for (int i = 0; i < m && done < n; i++)
    {
        int constraint = n;
        int freeBefore = n - done;
        for (int t = f - 1; t >= 0 && done < n; t--)
        {
            constraint = std::min(constraint, freeBefore);
            if (constraint == 0) break;

            freeBefore += doneTimeCounts[t] - reqTimeCounts[t];
            done++;
            doneTimeCounts[t]++;
            usedGpuCounts[i]++;
            s += gpuCosts[i];
            constraint--;
        }

        maxGpus = i + 1;
        donePerGpus.push_back(done);
        sPerGpus.push_back(s);
    }
}

std::vector<int> bestGroups;

int iters = 0;

void testGpus(int gpus, long long cumCost)
{
    int done = donePerGpus[gpus];
    long long baseS = sPerGpus[gpus];

    int baseRem = n - done;

    int slack = gpus < maxGpus ? std::max(usedGpuCounts[gpus] - 1, 0) : 0;
    int lTime = std::max(0, (baseRem - slack + gpus) / (gpus + 1));
    int rTime = baseRem / gpus;

    for (int time = lTime; time <= rTime; time++)
    {
        iters++;
        int f = maxTime + time + 1;
        long long s = baseS;
        int rem = baseRem;

        rem -= time * gpus;
        s += time * cumCost;

        if (gpus == m && rem > 0) continue;
        if (rem > 0) s += rem * gpuCosts[gpus];

        __int128 currAns = (__int128) f * s;

        if (currAns < ans)
        {
            bestGroups.clear();
        }

        ans = std::min(ans, currAns);

        if (currAns == ans)
        {
            bestGroups.push_back(gpus);
        }

        // std::clog << f << " * " << s << " = " << currAns << "\n";
    }
}

void solve()
{
    ans = (__int128) 4 * n * n * n;

    maxTime = reqTimes.back();

    reqTimeCounts.clear();
    reqTimeCounts.resize(maxTime + 1, 0);
    for (int time : reqTimes)
    {
        reqTimeCounts[time]++;
    }

    initBuild();

    long long cumCost = 0;
    for (int gpus = 1; gpus <= maxGpus; gpus++)
    {
        cumCost += gpuCosts[gpus - 1];
        testGpus(gpus, cumCost);
    }
}

__int128 solveGpus()
{
    m = gpuCosts.size();
    n = reqTimes.size();

    solve();

    return ans;
}

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
    int m;
    int maxTPow;
    int mode;

    int numTests;
};

constexpr long long rwBaseProb = 1;

void writeNumber(int num, std::ofstream& infile)
{
    std::string str = std::to_string(num);

    for (int i = 0; i < (int) str.size() - 1; ++i)
    {
        infile << "+";
    }

    infile << str;
}

void generate(TestParams test, bool useOptTarget, std::ofstream& infile)
{
    int n = test.n;
    int m = test.m;
    int maxT = 1;
    int mode = test.mode;

    for (int i = 0; i < test.maxTPow; ++i)
    {
        maxT *= 3;
    }

    assert(maxT <= n);

    if (maxT >= n / 4) useOptTarget = false;
    if (m < n) useOptTarget = false;

    int maxOptTarget = useOptTarget ? n / maxT * 0.8 : 0;

    if (maxOptTarget <= 5) useOptTarget = false;

    if (useOptTarget) std::cerr << "    maxTarget: " << maxOptTarget << std::endl;

    int optTarget = useOptTarget ? randInt(maxOptTarget * 9 / 10) + maxOptTarget / 10 : 0;

    int extra = useOptTarget ? 8 : 0;

    int retries = 0;

    while (true)
    {
        gpuCosts.clear();
        reqTimes.clear();

        long long currTotal = 0;
        for (int i = 0; i < m; ++i)
        {
            long long prevTargetTotal = (long long) i * i + (i < optTarget) * extra;
            long long targetTotal = (long long) (i + 1) * (i + 1) + (i + 1 < optTarget) * extra;

            long long prevDev = currTotal - prevTargetTotal;

            long long stepUProb = rwBaseProb + std::max(0ll, -prevDev);
            long long stepDProb = rwBaseProb + std::max(0ll, prevDev);
            long long step = oneIn(2) ? (randInt(stepUProb + stepDProb) < stepUProb ? 1 : -1) : 0;

            long long fullTargetTotal = targetTotal + prevDev + step;

            long long target = fullTargetTotal - currTotal;

            target = std::max<long long>(target, gpuCosts.empty() ? 1 : gpuCosts.back());
            target = std::min<long long>(target, 2 * n - 1);

            currTotal += target;

            gpuCosts.push_back(target);
        }

        std::sort(gpuCosts.begin(), gpuCosts.end());

        for (int i = 0; i < n; ++i)
        {
            if (mode == 0)
            {
                reqTimes.push_back(randInt(maxT));
            }
            else if (mode == 1)
            {
                long long u = randInt((long long) maxT * maxT);
                int time = sqrt(u);
                time = std::min(time, maxT - 1);
                time = std::max(time, 0);
                reqTimes.push_back(time);
            }
            else if (mode == 2)
            {
                long long u = randInt((long long) maxT * maxT);
                int time = sqrt(u);
                time = std::min(time, maxT - 1);
                time = std::max(time, 0);
                time = maxT - 1 - time;
                reqTimes.push_back(time);
            }
            else assert(false);
        }

        std::sort(reqTimes.begin(), reqTimes.end());

        if (!useOptTarget) break;

        std::cerr << "       Target: " << optTarget << std::endl;

        solveGpus();

        int bestGpus = bestGroups[0];

        std::cerr << "         Best: " << bestGpus << std::endl;

        if (bestGpus >= optTarget && bestGpus <= optTarget * 2) break;

        extra++;

        if (extra == 15)
        {
            std::cerr << "    FAILED" << std::endl;
            
            optTarget = randInt(maxOptTarget * 9 / 10) + maxOptTarget / 10;

            extra = 8;

            ++retries;

            if (retries == 10) break;
        }
    }

    infile << n << " " << m << "\n";

    for (int i = 0; i < m; ++i)
    {
        writeNumber(gpuCosts[i] - (i > 0 ? gpuCosts[i - 1] : 0), infile);
    }
    infile << "\n";

    for (int i = 0; i < n; ++i)
    {
        writeNumber(reqTimes[i] - (i > 0 ? reqTimes[i - 1] : 0), infile);
    }
    infile << "\n";
}

std::vector<TestParams> tests = {
    {      10,       10,   0,  0,  1}, //  // exp
    {      10,       10,   1,  1,  1}, //
    {      10,       10,   2,  2,  1}, //
    {      10,        2,   1,  1,  1}, //
    {     800,      800,   6,  2,  2}, //  // n3
    {     800,      800,   0,  0,  2}, //
    {     800,        5,   3,  1,  2}, //
    {     800,      800,   5,  1,  2}, //
    {     800,      800,   1,  1,  2}, //
    {     800,      800,   2,  1,  2}, //
    {     800,      800,   3,  1,  2}, //
    {     800,      800,   4,  1,  2}, //
    {    2200,     2200,   6,  2,  2}, //  // n2log
    {    2200,     2200,   0,  0,  2}, //
    {    2200,        5,   3,  1,  2}, //
    {    2200,     2200,   6,  1,  2}, //
    {    2200,     2200,   1,  1,  2}, //
    {    2200,     2200,   2,  1,  2}, //
    {    2200,     2200,   3,  1,  2}, //
    {    2200,     2200,   4,  0,  2}, //
    {    2200,     2200,   5,  1,  2}, //
    {   10000,    10000,   8,  2,  2}, //  // n2
    {   10000,    10000,   0,  0,  2}, //
    {   10000,        5,   4,  1,  2}, //
    {   10000,    10000,   6,  1,  2}, //
    {   10000,    10000,   1,  1,  2}, //
    {   10000,    10000,   2,  1,  2}, //
    {   10000,    10000,   3,  1,  2}, //
    {   10000,    10000,   4,  0,  2}, //
    {   10000,    10000,   5,  1,  2}, //
    {  100000,   100000,  10,  2,  2}, //  // n1log2
    {  100000,   100000,   0,  0,  2}, //
    {  100000,        5,   5,  1,  2}, //
    {  100000,   100000,   8,  1,  2}, //
    {  100000,   100000,   1,  1,  2}, //
    {  100000,   100000,   2,  1,  2}, //
    {  100000,   100000,   3,  1,  2}, //
    {  100000,   100000,   4,  1,  2}, //
    {  100000,   100000,   5,  0,  2}, //
    {  100000,   100000,   6,  1,  2}, //
    { 1000000,  1000000,  12,  2,  5}, //  // n1log
    { 1000000,  1000000,   0,  0,  5}, //
    { 1000000,        5,   6,  1,  5}, //
    { 1000000,  1000000,  10,  1,  5}, //
    { 1000000,  1000000,   1,  1,  5}, //
    { 1000000,  1000000,   2,  1,  5}, //
    { 1000000,  1000000,   3,  1,  5}, //
    { 1000000,  1000000,   4,  1,  5}, //
    { 1000000,  1000000,   5,  1,  5}, //
    { 1000000,  1000000,   6,  0,  5}, //
    { 1000000,  1000000,   8,  1,  5}, //
    {10000000, 10000000,  14,  2,  5}, //  // n1
    {10000000, 10000000,   0,  0,  5}, //
    {10000000,        5,   7,  1,  5}, //
    {10000000, 10000000,  12,  1,  5}, //
    {10000000, 10000000,   1,  1,  5}, //
    {10000000, 10000000,   2,  1,  5}, //
    {10000000, 10000000,   3,  1,  5}, //
    {10000000, 10000000,   4,  1,  5}, //
    {10000000, 10000000,   5,  1,  5}, //
    {10000000, 10000000,   6,  1,  5}, //
    {10000000, 10000000,   8,  0,  5}, //
    {10000000, 10000000,  10,  1,  5}, //
};

int main()
{
    generator.seed(1337);

    int num = 1;
    for (TestParams test : tests)
    {
        std::string nums = "";
        nums += num / 10 + '0';
        nums += num % 10 + '0';

        std::string inName = "gpus." + nums + ".in";

        std::cerr << "Test " << nums << std::endl;

        std::ofstream infile(inName);

        infile << test.numTests << "\n";

        for (int t = 0; t < test.numTests; ++t)
        {
            generate(test, t > test.numTests / 3, infile);
        }

        ++num;
    }

    return 0;
}
