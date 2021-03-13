#include <iostream>
#include <fstream>
#include <time.h>
#include <assert.h>
#include <algorithm>
#include <numeric>
#include <vector>
#include <unordered_set>
#include <random>

using namespace std;

const int MAX_N = 2e7;
const int MAX_XY = 1e8;

mt19937 generator;
int randNum(int lb, int ub)
{
    uniform_int_distribution<int> distribution(lb, ub);
    return distribution(generator);
}

bool oneIn(int chances)
{
    uniform_int_distribution<int> distribution(0, chances - 1);
    return distribution(generator) == 0;
}

int randWithRadius(int r)
{
    r = max(r, 0);
    int num = randNum(MAX_XY / 2 - r, MAX_XY / 2 + r);
    num = min(num, MAX_XY);
    num = max(num, 0);
    return num;
}

#define X_FULL 0
#define X_HALF 1

#define Y_RAND 0
#define Y_RAND_CNTR 1
#define Y_DECR_CNTR 2

int xType;
int yType;
int yCntr1, yCntr2;
bool swapXY;
double sharpness;

int n;

struct Point
{
    int x, y;
};

bool operator<(const Point& a, const Point& b)
{
    return a.x < b.x;
}

Point p[MAX_N];
vector<int> bigIdx, smallIdx;
int y2[MAX_N];

bool cmpSmallIdx(int a, int b)
{
    return abs(a - yCntr1) < abs(b - yCntr1);
}

bool cmpBigIdx(int a, int b)
{
    return abs(a - yCntr2) < abs(b - yCntr2);
}

void genTest(ofstream& outfile)
{
    for (int i = 0; i < n; ++i)
    {
        if (xType == X_FULL) p[i].x = randWithRadius(MAX_XY / 2);
        else if (xType == X_HALF) p[i].x = randWithRadius(MAX_XY / 4);
    }

    sort(p, p + n);

    if (yType == Y_RAND)
    {
        for (int i = 0; i < n; ++i)
        {
            p[i].y = randWithRadius(MAX_XY / 2);
        }
    }
    else if (yType == Y_RAND_CNTR)
    {
        if (yCntr1 > yCntr2) swap(yCntr1, yCntr2);
        int maxDist = yCntr1;
        maxDist = max(maxDist, n - 1 - yCntr2);
        maxDist = max(maxDist, (yCntr2 - yCntr1) / 2);
        for (int i = 0; i < n; ++i)
        {
            int dist = min(abs(yCntr1 - i), abs(yCntr2 - i));
            double ratio = 1.0 * dist / maxDist;
            ratio = pow(1 - ratio, sharpness);
            int rad = ratio * MAX_XY / 2;
            p[i].y = randWithRadius(rad);
        }
    }
    else if (yType == Y_DECR_CNTR)
    {
        smallIdx.clear();
        bigIdx.clear();
        for (int i = 0; i < n; ++i)
        {
            if (oneIn(2)) bigIdx.push_back(i);
            else smallIdx.push_back(i);
            y2[i] = randWithRadius(MAX_XY / 2);
        }

        sort(y2, y2 + n);
        sort(smallIdx.begin(), smallIdx.end(), cmpSmallIdx);
        sort(bigIdx.begin(), bigIdx.end(), cmpBigIdx);

        for (int i = 0; i < smallIdx.size(); ++i)
        {
            p[smallIdx[i]].y = y2[i];
        }

        for (int i = 0; i < bigIdx.size(); ++i)
        {
            p[bigIdx[i]].y = y2[n - 1 - i];
        }
    }

    if (swapXY)
    {
        for (int i = 0; i < n; ++i)
        {
            swap(p[i].x, p[i].y);
        }
        sort(p, p + n);
    }

    assert(n > 1);
    outfile << n << "\n";
    for (int i = 0 ; i < n; ++i)
    {
        assert(p[i].x >= 0 && p[i].x <= MAX_XY);
        assert(p[i].y >= 0 && p[i].y <= MAX_XY);
        if (i > 0) assert(p[i].x >= p[i - 1].x);
        outfile << p[i].x << " " << p[i].y << "\n";
    }
}

int main()
{
    generator.seed(time(nullptr));
    ios::sync_with_stdio(false);

    int num;
    //while (true)
    {
        /*cout << "Test num? ";
        cin >> num;
        if (num < 0) return 0;
        cout << "N? ";
        cin >> n;*/
        for (int k = 0; k < 6; ++k)
        {
            num = 1 + k * 15;
            if (k == 0) n = 25;
            else if (k == 1) n = 850;
            else if (k == 2) n = 19000;
            else if (k == 3) n = 90000;
            else if (k == 4) n = 610000;
            else if (k == 5) n = 2000000;

            for (int i = 0; i < 15; ++i)
            {
                if (i == 0)
                {
                    xType = X_FULL;
                    yType = Y_RAND_CNTR;
                    yCntr1 = yCntr2 = randNum(0, n - 1);
                    swapXY = false;
                    sharpness = 2.1;
                }
                else if (i == 1)
                {
                    xType = X_HALF;
                    yType = Y_RAND;
                    swapXY = false;
                }
                else if (i == 2)
                {
                    xType = X_FULL;
                    yType = Y_RAND_CNTR;
                    yCntr1 = yCntr2 = randNum(0, n - 1);
                    swapXY = true;
                    sharpness = 2.5;
                }
                else if (i == 3)
                {
                    xType = X_HALF;
                    yType = Y_RAND_CNTR;
                    yCntr1 = yCntr2 = randNum(0, n - 1);
                    swapXY = false;
                    sharpness = 3.1;
                }
                else if (i == 4)
                {
                    xType = X_FULL;
                    yType = Y_DECR_CNTR;
                    yCntr1 = randNum(0, n - 1);
                    yCntr2 = randNum(0, n - 1);
                    swapXY = false;
                }
                else if (i == 5)
                {
                    xType = X_FULL;
                    yType = Y_RAND_CNTR;
                    yCntr1 = yCntr2 = randNum(0, n - 1);
                    swapXY = true;
                    sharpness = 4.2;
                }
                else if (i == 6)
                {
                    xType = X_HALF;
                    yType = Y_RAND;
                    swapXY = true;
                }
                else if (i == 7)
                {
                    xType = X_FULL;
                    yType = Y_RAND_CNTR;
                    yCntr1 = randNum(0, n - 1);
                    yCntr2 = randNum(0, n - 1);
                    swapXY = false;
                    sharpness = 1.5;
                }
                else if (i == 8)
                {
                    xType = X_HALF;
                    yType = Y_RAND_CNTR;
                    yCntr1 = randNum(0, n - 1);
                    yCntr2 = randNum(0, n - 1);
                    swapXY = true;
                    sharpness = 2.7;
                }
                else if (i == 9)
                {
                    xType = X_HALF;
                    yType = Y_DECR_CNTR;
                    yCntr1 = randNum(0, n - 1);
                    yCntr2 = randNum(0, n - 1);
                    swapXY = false;
                }
                else if (i == 10)
                {
                    xType = X_FULL;
                    yType = Y_RAND_CNTR;
                    yCntr1 = yCntr2 = randNum(0, n - 1);
                    swapXY = false;
                    sharpness = 5.1;
                }
                else if (i == 11)
                {
                    xType = X_FULL;
                    yType = Y_RAND_CNTR;
                    yCntr1 = yCntr2 = randNum(0, n - 1);
                    swapXY = true;
                    sharpness = 5.2;
                }

                string nums = "";
                nums += (num / 10 + '0');
                nums += (num % 10 + '0');
                ofstream outfile("sets." + nums + ".in");
                genTest(outfile);
                outfile.close();

                ++num;
            }
        }
    }
    return 0;
}
