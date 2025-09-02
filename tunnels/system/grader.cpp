#include "tunnels.h"

#include <iostream>
#include <fstream>
#include <random>
#include <chrono>

static std::ofstream graderTimeFile;

static std::chrono::steady_clock::time_point startT;
static std::chrono::steady_clock::time_point endT;

static double totalGraderTime = 0;

static void startTime()
{
    startT = std::chrono::steady_clock::now();
}

static void writeTime()
{
    graderTimeFile << "io-time: " << totalGraderTime << std::endl;
}

static void endTime(bool write = true)
{
    endT = std::chrono::steady_clock::now();

    double time = std::chrono::duration_cast<std::chrono::duration<double>>(endT - startT).count();

    totalGraderTime += time;

    if (write)
    {
        writeTime();
    }
}

static const std::string PASSWORD = "4a89cneic0qkd0gd";
static const std::string CORRECT = "CORRECT";
static const std::string WRONG = "WRONG";

static void finish(bool correct)
{
    std::cout << PASSWORD << std::endl;
    if (correct) std::cout << CORRECT << std::endl;
    else std::cout << WRONG << std::endl;
    std::cout << totalGraderTime << std::endl;
    writeTime();
    graderTimeFile.close();
    exit(0);
}

static std::mt19937 generator;
static std::uniform_real_distribution<double> uniformDistr(0, 1);

static double randReal()
{
    return uniformDistr(generator);
}

static int seed;
static double leftProb;
static int maxCostDecr;
static bool useDiff;
static bool useLen;

static int n, m, k;
static std::vector<std::vector<bool>> blocked;

static const int BITS = 6;

static void input()
{
    std::cin >> seed >> leftProb >> maxCostDecr >> useDiff >> useLen;

    std::cin >> n >> m >> k;

    blocked.resize(n);
    for (int i = 0; i < n; i++)
    {
        blocked[i].resize(m);

        for (int j = 0; j < m; j += BITS)
        {
            char c;
            std::cin >> c;
            int code = c - '0';
            for (int j2 = 0; j2 < BITS && j + j2 < m; j2++)
            {
                blocked[i][j + j2] = (code >> j2) & 1;
            }
        }
    }
}

struct __Node
{
    int cost;
    int diff;

    int l, r;

    __Node* leftChild;
    __Node* rightChild;
};

static std::vector<std::vector<int>> dpCost;
static std::vector<std::vector<int>> dpDiff;

static int numNodes;
static std::vector<__Node> nodes;

static int targetQueries;

__Node* solveRow(int x, int minY, int maxY, bool calcDp = true)
{
    std::vector<__Node*> stack;

    auto mergeBack = [&]()
    {
        __Node* rightChild = stack.back();
        stack.pop_back();

        __Node* leftChild = stack.back();
        stack.pop_back();

        __Node* node = &nodes[numNodes++];

        node->cost = std::max(leftChild->cost, rightChild->cost) + 1;
        if (leftChild->cost > rightChild->cost) node->diff = leftChild->diff;
        else if (leftChild->cost < rightChild->cost) node->diff = rightChild->diff;
        else node->diff = std::max(leftChild->diff, rightChild->diff) + 1;
        node->l = leftChild->l;
        node->r = rightChild->r;
        node->leftChild = leftChild;
        node->rightChild = rightChild;

        stack.push_back(node);
    };

    for (int y = minY; y <= maxY; y++)
    {
        int newCost = dpCost[x + 1][y];
        if (newCost == -1) continue;

        while (stack.size() >= 2 && stack[stack.size() - 2]->cost <= newCost)
        {
            mergeBack();
        }

        while (stack.size() >= 2 && stack[stack.size() - 2]->cost == stack[stack.size() - 1]->cost)
        {
            mergeBack();
        }

        __Node* node = &nodes[numNodes++];

        node->cost = newCost;
        node->diff = dpDiff[x + 1][y];
        node->l = y;
        node->r = y;
        node->leftChild = nullptr;
        node->rightChild = nullptr;

        stack.push_back(node);

        if (stack.size() >= 2 && stack[stack.size() - 2]->cost <= stack[stack.size() - 1]->cost)
        {
            mergeBack();
        }

        while (stack.size() >= 2 && stack[stack.size() - 2]->cost == stack[stack.size() - 1]->cost)
        {
            mergeBack();
        }
    }

    while (stack.size() >= 2)
    {
        mergeBack();
    }

    __Node* root = nullptr;
    if (!stack.empty())
    {
        root = stack.back();
        stack.pop_back();
    }

    if (calcDp)
    {
        int cost = root ? root->cost : -1;
        int diff = root ? root->diff : -1;

        for (int y = minY; y <= maxY; y++)
        {
            dpCost[x][y] = cost;
            dpDiff[x][y] = diff;
        } 
    }

    return root;
};

static void prepare()
{
    generator.seed(seed);

    dpCost.resize(
        n + 1, std::vector<int>(m, -1)
    );

    dpDiff.resize(
        n + 1, std::vector<int>(m, -1)
    );

    for (int y = 0; y < m; y++)
    {
        dpCost[n][y] = 0;
        dpDiff[n][y] = 0;
    }

    nodes.resize(2 * m);

    for (int x = n - 1; x >= 0; x--)
    {
        int startY = -1;
        for (int endY = 0; endY <= m; endY++)
        {
            if (endY == m || blocked[x][endY])
            {
                if (startY + 1 < endY)
                {
                    numNodes = 0;
                    solveRow(x, startY + 1, endY - 1);
                }
                startY = endY;
            }
        }
    }

    targetQueries = dpCost[0][k];
}

static int numQueries = 0;

static int x;
static int enterY;
static int minY;
static int maxY;
static int currMinY;
static int currMaxY;

bool investigate(int y)
{
    startTime();

    numQueries++;

    if (x == n || y < minY || y > maxY || y == enterY)
    {
        finish(false);
    }

    int mid = y < enterY ? y : y - 1;

    if (mid < currMinY || mid >= currMaxY)
    {
        finish(false);
    }

    int lMaxY = mid;
    while (lMaxY > 0 && dpCost[x + 1][lMaxY] == -1) lMaxY--;

    int rMinY = mid + 1;
    while (rMinY < m - 1 && dpCost[x + 1][rMinY] == -1) rMinY++;

    numNodes = 0;
    __Node* left = solveRow(x, currMinY, lMaxY, false);
    __Node* right = solveRow(x, rMinY, currMaxY, false);

    if (numQueries + std::max(left->cost, right->cost) > targetQueries)
    {
        finish(false);
    }

    bool ansLeft = randReal() < leftProb;

    if (targetQueries - (numQueries + std::min(left->cost, right->cost)) > maxCostDecr)
    {
        if (left->cost > right->cost) ansLeft = true;
        else if (left->cost < right->cost) ansLeft = false;
    }
    else if (useDiff)
    {
        if (left->diff > right->diff) ansLeft = true;
        else if (left->diff < right->diff) ansLeft = false;
    }
    else if (useLen)
    {
        if (left->r - left->l > right->r - right->l) ansLeft = true;
        else if (left->r - left->l < right->r - right->l) ansLeft = false;
    }

    if (ansLeft) currMaxY = lMaxY;
    else currMinY = rMinY;

    bool ans = mid < enterY ? ansLeft : !ansLeft;

    endTime(false);

    return ans;
}

void goDeeper(int y)
{
    if (x == n || currMinY < currMaxY || y != currMinY)
    {
        finish(false);
    }

    x++;

    if (x == n) return;

    startTime();

    enterY = y;

    minY = y;
    while (minY > 0 && !blocked[x][minY - 1]) minY--;

    maxY = y;
    while (maxY < m - 1 && !blocked[x][maxY + 1]) maxY++;

    currMinY = minY;
    while (currMinY < m - 1 && dpCost[x + 1][currMinY] == -1) currMinY++;

    currMaxY = maxY;
    while (currMaxY > 0 && dpCost[x + 1][currMaxY] == -1) currMaxY--;

    endTime();
}

int main(int argc, char* argv[])
{
    graderTimeFile.open(argv[1]);

    startTime();

    input();

    prepare();

    x = -1;
    enterY = 0;

    minY = 0;
    maxY = m - 1;

    currMinY = k;
    currMaxY = k;

    endTime();

    goDeeper(k);

    solve(n, m, k, blocked);

    if (x < n)
    {
        finish(false);
    }

    finish(true);

    return 0;
}
