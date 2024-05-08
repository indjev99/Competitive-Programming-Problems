#include <iostream>
#include <vector>
#include <ctime>

#include "puzzle.h"

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
static int currSubtest;
static double timeLimit;

static void myAssert(bool f)
{
    if (f) return;

    std::cout << "Invalid solution on subtest: " << currSubtest + 1 <<  " / " << numSubtests << std::endl;
    std::cout << "Subtests solved: " << currSubtest << " / " << numSubtests << std::endl;
    std::cout << "Time taken: " << timePassed() << " / " << timeLimit << std::endl;

    exit(0);
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

    std::cerr << n << " " << m << std::endl;
    for (const std::vector<int>& row : puzzleOut)
    {
        for (int elem : row)
        {
            if (elem > 0 && elem <= 8) std::cerr << elem;
            else if (elem == EMPTY) std::cerr << " ";
            else if (elem == HOR_SINGLE) std::cerr << "-";
            else if (elem == HOR_DOUBLE) std::cerr << "=";
            else if (elem == VERT_SINGLE) std::cerr << "|";
            else if (elem == VERT_DOUBLE) std::cerr << "$";
            else std::cerr << "?";
        }
        std::cerr << std::endl;
    }
    std::cerr << std::endl;

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

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::cin >> numSubtests;
    std::cin >> timeLimit;

    startT = clock();

    init(numSubtests, timeLimit);

    for (currSubtest = 0; currSubtest < numSubtests; ++currSubtest)
    {
        input();
        if (!run())
        {
            std::cout << "Stopped on subtest: " << currSubtest + 1 << " / " << numSubtests << std::endl;
            break;
        }
        if (timePassed() > timeLimit)
        {
            std::cout << "Timed out during subtest: " << currSubtest + 1 << " / " << numSubtests << std::endl;
            break;
        }
    }

    std::cout << "Subtests solved: " << currSubtest << " / " << numSubtests << std::endl;
    std::cout << "Time taken: " << timePassed() << " / " << timeLimit << std::endl;

    return 0;
}
