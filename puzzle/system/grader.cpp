#include <iostream>
#include <vector>
#include <ctime>
// #include <thread>
// #include <chrono>
// #include <atomic>

#include "puzzle.h"

static const std::string password = "SD9G%^^H&O)UIKGH,QSAEE@R.QF42+]RW%$EGDTU&R-=1JG8HDS";
static constexpr int ALL = 0;
static constexpr int STOP = 1;
static constexpr int TIMEOUT = 2;
static constexpr int INVALID = 3;

static int n, m;
static std::vector<std::vector<int>> puzzle;
static std::vector<std::vector<int>> puzzleOut;

static void input()
{
    std::cin >> n >> m;

    puzzle.resize(n);
    for (int i = 0; i < n; ++i)
    {
        puzzle[i].resize(m);
        for (int j = 0; j < m; ++j)
        {
            std::cin >> puzzle[i][j];
        }
    }
}

static int numSubtests;
// static std::atomic<int> currSubtest;
static int currSubtest;
static double timeLimit;

// static std::atomic<bool> done = false;
// static std::atomic_flag doneFlag = ATOMIC_FLAG_INIT;

static void printResult(int verdict)
{
    // if (doneFlag.test_and_set()) return;

    // done = true;

    std::cout << password << std::endl;
    std::cout << verdict << " " << currSubtest << " " << timePassed() << std::endl;
}

static void myAssert(bool f)
{
    if (f) return;

    throw INVALID;
}

static std::vector<std::vector<bool>> visited;

static void DFS(int i, int j)
{
    if (i < 0) return;
    if (i >= n) return;
    if (j < 0) return;
    if (j >= m) return;

    if (puzzleOut[i][j] == EMPTY) return;
    if (visited[i][j]) return;

    visited[i][j] = true;

    DFS(i - 1, j);
    DFS(i + 1, j);
    DFS(i, j - 1);
    DFS(i, j + 1);
}

static bool run()
{
    puzzleOut = puzzle;
    bool res = solve(puzzleOut);

    if (!res) return false;

    myAssert((int) puzzleOut.size() == n);
    for (int i = 0; i < n; ++i)
    {
        myAssert((int) puzzleOut[i].size() == m);
        for (int j = 0; j < m; ++j)
        {
            if (puzzle[i][j] > 0)
            {
                myAssert(puzzleOut[i][j] == puzzle[i][j]);
            }
            else
            {
                myAssert(
                    puzzleOut[i][j] == EMPTY ||
                    puzzleOut[i][j] == HOR_SINGLE ||
                    puzzleOut[i][j] == VERT_SINGLE ||
                    puzzleOut[i][j] == HOR_DOUBLE ||
                    puzzleOut[i][j] == VERT_DOUBLE);
            }
        }
    }

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (puzzleOut[i][j] > 0)
            {
                int sum = 0;
                if (j > 0 && puzzleOut[i][j - 1] == HOR_SINGLE) sum += 1;
                if (j > 0 && puzzleOut[i][j - 1] == HOR_DOUBLE) sum += 2;
                if (j < m - 1 && puzzleOut[i][j + 1] == HOR_SINGLE) sum += 1;
                if (j < m - 1 && puzzleOut[i][j + 1] == HOR_DOUBLE) sum += 2;
                if (i > 0 && puzzleOut[i - 1][j] == VERT_SINGLE) sum += 1;
                if (i > 0 && puzzleOut[i - 1][j] == VERT_DOUBLE) sum += 2;
                if (i < n - 1 && puzzleOut[i + 1][j] == VERT_SINGLE) sum += 1;
                if (i < n - 1 && puzzleOut[i + 1][j] == VERT_DOUBLE) sum += 2;
                myAssert(sum == puzzleOut[i][j]);
            }
            else if (puzzleOut[i][j] != EMPTY)
            {
                if (puzzleOut[i][j] == HOR_SINGLE)
                {
                    myAssert(j > 0 && (puzzleOut[i][j - 1] == HOR_SINGLE || puzzleOut[i][j - 1] > 0));
                    myAssert(j < m - 1 && (puzzleOut[i][j + 1] == HOR_SINGLE || puzzleOut[i][j + 1] > 0));
                }
                if (puzzleOut[i][j] == HOR_DOUBLE)
                {
                    myAssert(j > 0 && (puzzleOut[i][j - 1] == HOR_DOUBLE || puzzleOut[i][j - 1] > 0));
                    myAssert(j < m - 1 && (puzzleOut[i][j + 1] == HOR_DOUBLE || puzzleOut[i][j + 1] > 0));
                }
                if (puzzleOut[i][j] == VERT_SINGLE)
                {
                    myAssert(i > 0 && (puzzleOut[i - 1][j] == VERT_SINGLE || puzzleOut[i - 1][j] > 0));
                    myAssert(i < n - 1 && (puzzleOut[i + 1][j] == VERT_SINGLE || puzzleOut[i + 1][j] > 0));
                }
                if (puzzleOut[i][j] == VERT_DOUBLE)
                {
                    myAssert(i > 0 && (puzzleOut[i - 1][j] == VERT_DOUBLE || puzzleOut[i - 1][j] > 0));
                    myAssert(i < n - 1 && (puzzleOut[i + 1][j] == VERT_DOUBLE || puzzleOut[i + 1][j] > 0));
                }
            }
        }
    }

    visited.clear();
    visited.resize(n);
    for (int i = 0; i < n; ++i)
    {
        visited[i].resize(m, false);
    }

    bool found = false;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            if (puzzleOut[i][j] == EMPTY) continue;
            if (!found)
            {
                DFS(i, j);
                found = true;
            }
            myAssert(visited[i][j]);
        }
    }

    return true;
}

static clock_t startT;

double timePassed()
{
    clock_t currT = clock();

    return (double) (currT - startT) / CLOCKS_PER_SEC;
}

void mainRun()
{
    for (currSubtest = 0; currSubtest < numSubtests; ++currSubtest)
    {
        input();
        try
        {
            int res = run();
            if (!run())
            {
                printResult(STOP);
                return;
            }
            if (timePassed() > timeLimit)
            {
                printResult(TIMEOUT);
                return;
            }
        }
        catch (int e)
        {
            printResult(INVALID);
            return;
        }
    }

    printResult(ALL);
    return;
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> numSubtests;
    std::cin >> timeLimit;

    startT = clock();

    init(numSubtests, timeLimit);

    mainRun();

    // std::thread mainRunThread(mainRun);
    // mainRunThread.detach();

    // while (timePassed() < timeLimit + 0.5 && !done)
    // {
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500));
    // }

    // if (!done) printResult(TIMEOUT);

    return 0;
}
